/**
 * ReflectionGen -- C++20 header scanner + code generator for Parasite Engine's
 * reflection system. See Parser.h for the grammar this understands.
 *
 * Usage:
 *     ReflectionGen --scan <directory> [<directory> ...] --output <directory>
 */

#include "algorithm"
#include "filesystem"
#include "fstream"
#include "iostream"
#include "sstream"

#include "CodeGen.h"
#include "Lexer.h"
#include "Parser.h"


std::string ReadFile(const std::filesystem::path& InPath)
{
	std::ifstream Ifs(InPath, std::ios::binary);
	std::ostringstream Buffer;
	Buffer << Ifs.rdbuf();
	return Buffer.str();
}

bool HasGeneratedSuffix(const std::string & InFilename)
{
	const std::string Suffix = ".generated.h";
	return InFilename.size() >= Suffix.size() &&
		InFilename.compare(InFilename.size() - Suffix.size(), Suffix.size(), Suffix) == 0;
}

std::vector<std::filesystem::path> FindReflectedHeaders(const std::filesystem::path & InRoot)
{
	std::vector<std::filesystem::path> Headers;
	if (!std::filesystem::exists(InRoot))
	{
		return Headers;
	}

	for (const std::filesystem::directory_entry& Entry : std::filesystem::recursive_directory_iterator(InRoot))
	{
		if (!Entry.is_regular_file() || Entry.path().extension() != ".h")
		{
			continue;
		}
		const std::string Filename = Entry.path().filename().string();
		if (HasGeneratedSuffix(Filename))
		{
			continue;
		}

		const std::string Text = ReadFile(Entry.path());
		if (Text.find("PE_CLASS(") != std::string::npos || Text.find("PE_ENUM(") != std::string::npos)
		{
			Headers.push_back(Entry.path());
		}
	}

	std::sort(Headers.begin(), Headers.end());
	return Headers;
}


int main(int InArgCount, char** InArgValues)
{
    std::vector<std::string> Args(InArgValues + 1, InArgValues + InArgCount);

    auto ScanIt = std::find(Args.begin(), Args.end(), "--scan");
    if (ScanIt == Args.end())
    {
        std::cout << "Must include a scan directory! Usage: ReflectionGen --scan <directories...> --output <directory>\n";
        return 1;
    }

    auto OutputIt = std::find(Args.begin(), Args.end(), "--output");

    std::vector<std::filesystem::path> ScanPaths;
    for (auto It = ScanIt + 1; It != Args.end() && It != OutputIt; ++It)
    {
        ScanPaths.emplace_back(*It);
    }

    std::filesystem::path OutputDir = "Generated";
    if (OutputIt != Args.end() && (OutputIt + 1) != Args.end())
    {
        OutputDir = *(OutputIt + 1);
    }

    std::cout << "Scanning Directories:\n";
    std::vector<std::filesystem::path> Targets;
    for (const std::filesystem::path& Root : ScanPaths)
    {
        std::error_code ErrorCode;
        std::cout << "  " << std::filesystem::absolute(Root, ErrorCode).string() << "\n";
        std::vector<std::filesystem::path> Found = FindReflectedHeaders(Root);
        Targets.insert(Targets.end(), Found.begin(), Found.end());
    }

    if (Targets.empty())
    {
        std::cout << "No PE_CLASS()/PE_ENUM() headers found\n";
        return 0;
    }

    for (const std::filesystem::path& Header : Targets)
    {
        std::cout << "Begin reflection: " << Header.string() << "\n";
        const std::string Source = ReadFile(Header);
        const std::vector<EToken> Tokens = Tokenize(Source);
        const SParsedFile Parsed = Parse(Tokens);

        if (auto OutputPath = GenerateOutput(Header, Parsed, OutputDir))
        {
            std::cout << "Generated: " << OutputPath->string() << "\n";
        }
    }

    return 0;
}
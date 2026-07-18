#include "Lexer.h"

#include "cctype"
#include "algorithm"

std::vector<EToken> Tokenize(const std::string& InSource)
{
	std::vector<EToken> Tokens;
	const size_t Length = InSource.size();
	size_t Index = 0;
	int Line = 1;
	bool bAtLineStart = true;

	auto IsIdentStart = [](char InCharacter) { return std::isalpha(static_cast<unsigned char>(InCharacter)) || InCharacter == '_'; };
	auto IsIdentChar = [](char InCharacter) { return std::isalnum(static_cast<unsigned char>(InCharacter)) || InCharacter == '_'; };

	while (Index < Length)
	{
		const char Character = InSource[Index];

		if (Character == '\n')
		{
			++Line;
			++Index;
			bAtLineStart = true;
			continue;
		}

		if (std::isspace(static_cast<unsigned char>(Character)))
		{
			++Index;
			continue;
		}

		// Preprocessor directive: '#' as the first non-whitespace char on a
		// line. Swallow the whole logical line (honouring backslash line
		// continuations) without tokenizing its contents at all.
		if (Character == '#' && bAtLineStart)
		{
			while (Index < Length && InSource[Index] != '\n')
			{
				if (InSource[Index] == '\\' && Index + 1 < Length && InSource[Index + 1] == '\n')
				{
					++Line;
					Index += 2;
					continue;
				}
				++Index;
			}
			continue; // leave the '\n' itself for the top of the loop to consume
		}

		bAtLineStart = false;

		// Line comment
		if (Character == '/' && Index + 1 < Length && InSource[Index + 1] == '/')
		{
			Index += 2;
			while (Index < Length && InSource[Index] != '\n')
			{
				++Index;
			}
			continue;
		}

		// Block comment
		if (Character == '/' && Index + 1 < Length && InSource[Index + 1] == '*')
		{
			Index += 2;
			while (Index + 1 < Length && !(InSource[Index] == '*' && InSource[Index + 1] == '/'))
			{
				if (InSource[Index] == '\n')
				{
					++Line;
				}
				++Index;
			}
			Index = std::min(Index + 2, Length);
			continue;
		}

		// String literal
		if (Character == '"')
		{
			const int StartLine = Line;
			std::string Text;
			Text.push_back(Character);
			++Index;
			while (Index < Length && InSource[Index] != '"')
			{
				if (InSource[Index] == '\\' && Index + 1 < Length)
				{
					Text.push_back(InSource[Index]);
					Text.push_back(InSource[Index + 1]);
					Index += 2;
					continue;
				}
				if (InSource[Index] == '\n')
				{
					++Line;
				}
				Text.push_back(InSource[Index]);
				++Index;
			}
			if (Index < Length)
			{
				Text.push_back('"');
				++Index;
			}
			Tokens.push_back({ ETokenKind::StringLiteral, Text, StartLine });
			continue;
		}

		// Char literal
		if (Character == '\'')
		{
			const int StartLine = Line;
			std::string Text;
			Text.push_back(Character);
			++Index;
			while (Index < Length && InSource[Index] != '\'')
			{
				if (InSource[Index] == '\\' && Index + 1 < Length)
				{
					Text.push_back(InSource[Index]);
					Text.push_back(InSource[Index + 1]);
					Index += 2;
					continue;
				}
				Text.push_back(InSource[Index]);
				++Index;
			}
			if (Index < Length)
			{
				Text.push_back('\'');
				++Index;
			}
			Tokens.push_back({ ETokenKind::CharLiteral, Text, StartLine });
			continue;
		}

		// Number
		if (std::isdigit(static_cast<unsigned char>(Character)))
		{
			const int StartLine = Line;
			std::string Text;
			while (Index < Length && (std::isalnum(static_cast<unsigned char>(InSource[Index])) || InSource[Index] == '.' || InSource[Index] == '\''))
			{
				Text.push_back(InSource[Index]);
				++Index;
			}
			Tokens.push_back({ ETokenKind::Number, Text, StartLine });
			continue;
		}

		// Identifier / keyword
		if (IsIdentStart(Character))
		{
			const int StartLine = Line;
			std::string Text;
			while (Index < Length && IsIdentChar(InSource[Index]))
			{
				Text.push_back(InSource[Index]);
				++Index;
			}
			Tokens.push_back({ ETokenKind::Identifier, Text, StartLine });
			continue;
		}

		// Scope resolution "::"
		if (Character == ':' && Index + 1 < Length && InSource[Index + 1] == ':')
		{
			Tokens.push_back({ ETokenKind::ScopeResolution, "::", Line });
			Index += 2;
			continue;
		}

		// Everything else: one punctuation character per token. This keeps
		// nested templates ("vector<vector<int>>") trivially balanceable
		// without special-casing ">>" as a shift operator.
		Tokens.push_back({ ETokenKind::Punct, std::string(1, Character), Line });
		++Index;
	}

	Tokens.push_back({ ETokenKind::EndOfFile, "", Line });
	return Tokens;
}
#pragma once

#include "string"
#include "filesystem"

namespace Parasite
{
	class CFileDialogs
	{
	public:
		static std::string OpenFile(const char* InFilter);
		static std::string SaveFile(const char* InFilter);

		static void OpenFolder(const std::filesystem::path& InPath);
	};
}
#include "pepch.h"
#include "ParasiteEngine/Utils/PlatformUtils.h"

#include "ParasiteEngine/Core/Application.h"

#include "commdlg.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"


namespace Parasite
{
	std::string CFileDialogs::OpenFile(const char* InFilter)
	{
		OPENFILENAMEA OFN;
		CHAR SzFile[256] = { 0 };

		ZeroMemory(&OFN, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow()));
		OFN.lpstrFile = SzFile;
		OFN.nMaxFile = sizeof(SzFile);
		OFN.lpstrFilter = InFilter;
		OFN.nFilterIndex = 1;
		OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&OFN) == TRUE)
		{
			return OFN.lpstrFile;
		}
		return std::string();
	}

	std::string CFileDialogs::SaveFile(const char* InFilter)
	{
		OPENFILENAMEA OFN;
		CHAR SzFile[256] = { 0 };

		ZeroMemory(&OFN, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow()));
		OFN.lpstrFile = SzFile;
		OFN.nMaxFile = sizeof(SzFile);
		OFN.lpstrFilter = InFilter;
		OFN.nFilterIndex = 1;
		OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&OFN) == TRUE)
		{
			return OFN.lpstrFile;
		}
		return std::string();
	}
}
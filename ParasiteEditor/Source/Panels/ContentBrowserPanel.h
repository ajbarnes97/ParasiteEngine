#pragma once
#include "ParasiteEngine.h"

#include "filesystem"

class CTexture2D;


namespace Parasite
{
	class CContentBrowserPanel
	{
	public:
		CContentBrowserPanel();

		void OnImGuiRender();

	private:
		void DrawDirectoryBox();
		void DrawSettingsContextMenu();
		void DrawThumbnails();

	private:
		std::filesystem::path CurrentDirectory;
		std::filesystem::path InputDirectory;

		float ThumbnailSize = 64.0f;
		float Padding = 8.0f;

		TSharedPtr<CTexture2D> FolderIcon;
		TSharedPtr<CTexture2D> FileIcon;
		TSharedPtr<CTexture2D> SettingsIcon;
	};
}
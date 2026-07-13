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
		void DrawDirectoryTreeFromPath(const std::filesystem::path& Path, bool bInExpandRoot = false);

		void DrawDirectoryListView();
		void DrawDirectoryHistory();
		void DrawDirectoryInputBox();
		void DrawSettingsContextMenu();
		void DrawThumbnails();

		void DrawFolderContextMenu(const std::filesystem::path& InPath);

		void NavigateTo(const std::filesystem::path& InPath);
		inline bool CanGoBack() const { return HistoryIndex > 0; };
		inline bool CanGoForward() const { return HistoryIndex + 1 < NavHistory.size(); }

	private:
		std::filesystem::path CurrentDirectory;
		std::filesystem::path InputDirectory;

		std::vector<std::filesystem::path> NavHistory;
		int32_t HistoryIndex = INDEX_NONE;

		float ThumbnailSize = 64.0f;
		float Padding = 16.0f;

		TSharedPtr<CTexture2D> FolderIcon;
		TSharedPtr<CTexture2D> FileIcon;
		TSharedPtr<CTexture2D> SettingsIcon;
		TSharedPtr<CTexture2D> ArrowIcon;
	};
}
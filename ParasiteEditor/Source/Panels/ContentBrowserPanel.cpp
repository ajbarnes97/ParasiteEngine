#include "pepch.h"
#include "ContentBrowserPanel.h"

#include "ParasiteEngine/Renderer/Texture.h"
#include "ParasiteEngine/Utils/PlatformUtils.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"


namespace Parasite
{
	// todo: should be based on projects dir
	extern const std::filesystem::path AssetPath = "Assets";

	CContentBrowserPanel::CContentBrowserPanel()
	{
		CurrentDirectory = AssetPath;
		InputDirectory = CurrentDirectory.string();
		NavHistory.push_back(CurrentDirectory);
		HistoryIndex = 0;

		FolderIcon = CTexture2D::Create("Resources/Icons/folder.png");
		FileIcon = CTexture2D::Create("Resources/Icons/file.png");
		SettingsIcon = CTexture2D::Create("Resources/Icons/settings.png");
		ArrowIcon = CTexture2D::Create("Resources/Icons/arrow.png");
	}

	void CContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("ContentBrowser");
		ImGui::BeginTable("ContentBrowser", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV);

		ImGui::TableSetupColumn("DirectoryView", ImGuiTableColumnFlags_WidthFixed, 250.0f);
		ImGui::TableSetupColumn("Content");
		
		DrawDirectoryListView();
		DrawDirectoryHistory();
		DrawDirectoryInputBox();
		DrawSettingsContextMenu();
		DrawThumbnails();

		ImGui::EndTable();
		ImGui::End();
	}

	void CContentBrowserPanel::DrawDirectoryListView()
	{
		ImGui::TableNextColumn();
		ImGui::BeginChild("DirectoryPanel", ImVec2(0, 0), false);

		ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

		if (ImGui::TreeNodeEx("Content", TreeNodeFlags))
		{
			DrawDirectoryTreeFromPath(AssetPath, true);
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Favorites", TreeNodeFlags))
		{
			ImGui::TreePop();
		}
		ImGui::EndChild();
	}

	void CContentBrowserPanel::DrawDirectoryTreeFromPath(const std::filesystem::path& InPath, bool bInExpandRoot)
	{
		bool bHasChildren = false;
		for (const auto& DirEntry : std::filesystem::directory_iterator(InPath))
		{
			if (DirEntry.is_directory())
			{
				bHasChildren = true;
				break;
			}
		}

		ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (!bHasChildren)
		{
			TreeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
		}
		else if (bInExpandRoot)
		{
			TreeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
		}

		if (CurrentDirectory == InPath)
		{
			TreeNodeFlags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen;
		}

		const bool bOpen = ImGui::TreeNodeEx(InPath.filename().string().c_str(), TreeNodeFlags);
		if (ImGui::IsItemClicked())
		{
			NavigateTo(InPath);
		}
		DrawFolderContextMenu(InPath);

		if (bOpen)
		{
			for (const auto& DirEntry : std::filesystem::directory_iterator(InPath))
			{
				if (DirEntry.is_directory())
				{
					DrawDirectoryTreeFromPath(DirEntry.path());
				}
			}
			ImGui::TreePop();
		}
	}

	void CContentBrowserPanel::DrawDirectoryHistory()
	{
		ImGui::TableNextColumn();

		constexpr float ButtonSize = 16.0f;
		if (ImGui::ImageButton("NavPrevious", ArrowIcon->GetRendererID(), { ButtonSize, ButtonSize }))
		{
			if (CanGoBack())
			{
				HistoryIndex--;
				NavigateTo(NavHistory[HistoryIndex]);
			}
		}
		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		if (ImGui::ImageButton("NavNext", ArrowIcon->GetRendererID(), { ButtonSize, ButtonSize }, { 1, 0 }, { 0, 1 }))
		{
			if (CanGoForward())
			{
				HistoryIndex++;
				NavigateTo(NavHistory[HistoryIndex]);
			}
		}
	}

	void CContentBrowserPanel::DrawDirectoryInputBox()
	{
		char Buffer[512];
		memset(Buffer, 0, sizeof(Buffer));
		strcpy_s(Buffer, InputDirectory.string().c_str());

		ImGui::SameLine();
		if (ImGui::InputText("##Directory", Buffer, sizeof(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::filesystem::path NewPath(Buffer);

			if (std::filesystem::exists(NewPath))
			{
				auto CanonicalAssets = std::filesystem::canonical(AssetPath);
				auto CanonicalNew = std::filesystem::canonical(NewPath);

				auto AssetsStr = CanonicalAssets.string();
				auto NewStr = CanonicalNew.string();

				if (NewStr.starts_with(AssetsStr))
				{
					CurrentDirectory = CanonicalNew;
					InputDirectory = std::filesystem::relative(CurrentDirectory, std::filesystem::current_path());
				}
			}
			else
			{
				InputDirectory = std::filesystem::relative(CurrentDirectory, std::filesystem::current_path());;
			}
		}
		else
		{
			InputDirectory = Buffer;
		}
	}

	void CContentBrowserPanel::DrawSettingsContextMenu()
	{
		ImGui::SameLine();
		if (ImGui::ImageButton("DirectoryMenu", SettingsIcon->GetRendererID(), { 16.0f, 16.0f }))
		{
			ImGui::OpenPopup("DirectoryMenu");
		}

		if (ImGui::BeginPopup("DirectoryMenu"))
		{
			ImGui::SliderFloat("Thumbnail Size", &ThumbnailSize, 16.0f, 256.0f);
			ImGui::SliderFloat("Padding", &Padding, 16.0f, 256.0f);
			ImGui::Separator();
			
			if (ImGui::MenuItem("Show in Explorer"))
			{
				CFileDialogs::OpenFolder(CurrentDirectory);
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Copy Path"))
			{
				ImGui::SetClipboardText(CurrentDirectory.string().c_str());
			}

			ImGui::EndPopup();
		}
	}

	void CContentBrowserPanel::DrawThumbnails()
	{
		const float CellSize = Padding + ThumbnailSize;
		const float PanelWidth = ImGui::GetContentRegionAvail().x;
		const int ColumnCount = std::max(1, static_cast<int>(PanelWidth / CellSize));

		if (ImGui::BeginTable("ContentBrowserGrid", ColumnCount, ImGuiTableFlags_SizingFixedFit))
		{
			for (auto& DirEntry : std::filesystem::directory_iterator(CurrentDirectory))
			{
				ImGui::TableNextColumn();

				const auto& Path = DirEntry.path();
				auto RelativePath = std::filesystem::relative(Path, AssetPath);
				std::string FilenameString = RelativePath.filename().string();
				ImGui::PushID(FilenameString.c_str());

				const bool bIsDirectory = DirEntry.is_directory();
				ImTextureID TextureID = bIsDirectory ? FolderIcon->GetRendererID() : FileIcon->GetRendererID();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton(FilenameString.c_str(), TextureID, { ThumbnailSize, ThumbnailSize }, { 0, 1 }, { 1, 0 });

				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* ItemPath = RelativePath.c_str(); 
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", ItemPath, (wcslen(ItemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (bIsDirectory)
					{
						NavigateTo(CurrentDirectory / Path.filename());
					}
					else
					{
						// TODO: Open asset
					}
				}

				if (bIsDirectory)
				{
					DrawFolderContextMenu(Path);
				}

				ImGui::TextWrapped("%s", FilenameString.c_str());
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}

	void CContentBrowserPanel::DrawFolderContextMenu(const std::filesystem::path& InPath)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Show in Explorer"))
			{
				CFileDialogs::OpenFolder(InPath);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Add To Favorites"))
			{
				// todo: save to editor settings
			}
			ImGui::EndPopup();
		}
	}

	void CContentBrowserPanel::NavigateTo(const std::filesystem::path& InPath)
	{
		if (HistoryIndex >= 0 && NavHistory[HistoryIndex] == InPath)
		{
			CurrentDirectory = InPath;
			InputDirectory = InPath;
			return;
		}

		if (HistoryIndex + 1 < static_cast<int32_t>(NavHistory.size()))
		{
			NavHistory.erase(NavHistory.begin() + HistoryIndex + 1, NavHistory.end());
		}

		NavHistory.push_back(InPath);
		HistoryIndex = static_cast<int32_t>(NavHistory.size()) - 1;

		CurrentDirectory = InPath;
		InputDirectory = InPath;
	}
}
#include "pepch.h"
#include "ContentBrowserPanel.h"

#include "ParasiteEngine/Renderer/Texture.h"
#include "ParasiteEngine/Utils/PlatformUtils.h"

#include "ImGui/imgui.h"


namespace Parasite
{
	// todo: should be based on projects dir
	static const std::filesystem::path AssetPath = "Assets";

	CContentBrowserPanel::CContentBrowserPanel()
	{
		CurrentDirectory = AssetPath;
		InputDirectory = CurrentDirectory.string();

		FolderIcon = CTexture2D::Create("Resources/Icons/folder.png");
		FileIcon = CTexture2D::Create("Resources/Icons/file.png");
		SettingsIcon = CTexture2D::Create("Resources/Icons/settings.png");
	}

	void CContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (CurrentDirectory != AssetPath)
		{
			if (ImGui::Button("<-"))
			{
				CurrentDirectory = CurrentDirectory.parent_path();
			}
			ImGui::SameLine();
		}

		DrawDirectoryBox();
		DrawSettingsContextMenu();
		DrawThumbnails();

		ImGui::End();
	}

	void CContentBrowserPanel::DrawDirectoryBox()
	{
		char Buffer[512];
		memset(Buffer, 0, sizeof(Buffer));
		strcpy_s(Buffer, InputDirectory.string().c_str());

		ImGui::TextUnformatted("Directory");
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
		const int ColumnCount = static_cast<int>(PanelWidth / CellSize);

		ImGui::Columns(ColumnCount <= 0 ? 1 : ColumnCount, 0, false);
		for (auto& DirEntry : std::filesystem::directory_iterator(CurrentDirectory))
		{
			const auto& Path = DirEntry.path();
			auto RelativePath = std::filesystem::relative(Path, AssetPath);
			std::string FilenameString = RelativePath.filename().string();

			const bool bIsDirectory = DirEntry.is_directory();

			// todo: have different icons for different file extensions
			ImTextureID TextureID = bIsDirectory ? FolderIcon->GetRendererID() : FileIcon->GetRendererID();
			ImGui::ImageButton(FilenameString.c_str(), TextureID, { ThumbnailSize, ThumbnailSize }, { 0, 1 }, { 1,0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (bIsDirectory)
				{
					CurrentDirectory /= Path.filename();
					InputDirectory /= Path.filename();
				}
				else
				{
					// todo: handle opening files
				}
			}
			ImGui::Text(FilenameString.c_str());
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}
}
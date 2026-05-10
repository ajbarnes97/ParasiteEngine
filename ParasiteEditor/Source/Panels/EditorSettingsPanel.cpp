#include "pepch.h"
#include "EditorSettingsPanel.h"


namespace Parasite
{
	void CEditorSettingsPanel::ShowWindow(SEditorSettings* InSettings)
	{
		bWindowOpen = true;
		EditorSettings = InSettings;
	}

	void CEditorSettingsPanel::OnImGuiRender()
	{
		if (!bWindowOpen)
		{
			return;
		}

		ImGui::SetNextWindowSize(ImVec2(800, 440), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Editor Settings", &bWindowOpen, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Close"))
					{
						bWindowOpen = false;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			int Selected = 0;

			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

			char Lable[128];
			sprintf(Lable, "Appearance");
			ImGui::Selectable(Lable, Selected == 0, ImGuiSelectableFlags_SelectOnNav);
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

			switch (Selected)
			{
			case 0:
			{
				DrawCategoryAppearance();
				break;
			}
			default:
			{
				PE_CORE_ERROR("Editor settings category not implemented.");
			}
			}

			ImGui::EndChild();

			if (ImGui::Button("Apply", ImVec2(120.0f, 30.0f)))
			{
				OnEditorSettingsApplied(EditorSettings);
			}

			ImGui::EndGroup();
		}
		ImGui::End();
	}

	void CEditorSettingsPanel::DrawCategoryAppearance()
	{
		ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::CollapsingHeader("Main", Flags))
		{
			DrawColour("Background", EditorSettings->Background, OnStyleChanged);
			DrawColour("Border", EditorSettings->Border, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Headers", Flags))
		{
			DrawColour("Header", EditorSettings->Header, OnStyleChanged);
			DrawColour("Header Hovered", EditorSettings->HeaderHovered, OnStyleChanged);
			DrawColour("Header Active", EditorSettings->HeaderActive, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Buttons", Flags))
		{
			DrawColour("Button", EditorSettings->Button, OnStyleChanged);
			DrawColour("Button Hovered", EditorSettings->ButtonHovered, OnStyleChanged);
			DrawColour("Button Active", EditorSettings->ButtonActive, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Frame", Flags))
		{
			DrawColour("Frame Background", EditorSettings->FrameBackground, OnStyleChanged);
			DrawColour("Frame Hovered", EditorSettings->FrameHovered, OnStyleChanged);
			DrawColour("Frame Active", EditorSettings->FrameActive, OnStyleChanged);
		}
	}
}
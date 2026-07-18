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
				EditorUI::DrawReflectedProperties(EditorSettings, EditorSettings->GetClass());
				break;
			}
			default:
			{
				PE_CORE_ERROR("Editor settings category not implemented.");
			}
			}

			ImGui::EndChild();

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + 45.0f);
			if (ImGui::Button("Apply", ImVec2(120.0f, 25.0f)))
			{
				OnEditorSettingsApplied(EditorSettings);
			}

			ImGui::EndGroup();
		}
		ImGui::End();
	}
}
#pragma once
#include "ParasiteEngine.h"

#include "ImGui/imgui.h"


namespace Parasite
{
	class CEditorSettings
	{
	public:
		CEditorSettings() = default;

		void ShowWindow() { bWindowOpen = true; }
		void OnImGuiRender()
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

				// Left
				static int selected = 0;
				{
					ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
					for (int i = 0; i < 100; i++)
					{
						char label[128];
						sprintf(label, "MyObject %d", i);
						if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SelectOnNav))
							selected = i;
					}
					ImGui::EndChild();
				}
				ImGui::SameLine();

				{
					ImGui::BeginGroup();
					ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
					ImGui::Text("MyObject: %d", selected);
					ImGui::Separator();
					if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
					{
						if (ImGui::BeginTabItem("Description"))
						{
							ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Details"))
						{
							ImGui::Text("ID: 0123456789");
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}

			}
			ImGui::End();
		}

	private:
		bool bWindowOpen = false;
	};
}
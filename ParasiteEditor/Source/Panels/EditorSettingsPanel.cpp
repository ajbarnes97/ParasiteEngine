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

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + 45.0f);
			if (ImGui::Button("Apply", ImVec2(120.0f, 25.0f)))
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

			DrawColour("Child Background", EditorSettings->ChildBackground, OnStyleChanged);
			DrawColour("Popup Background", EditorSettings->PopupBackground, OnStyleChanged);
			DrawColour("Border Shadow", EditorSettings->BorderShadow, OnStyleChanged);
			DrawColour("Menu Bar Background", EditorSettings->MenuBarBackground, OnStyleChanged);
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

		if (ImGui::CollapsingHeader("Tabs", Flags))
		{
			DrawColour("Tab", EditorSettings->Tab, OnStyleChanged);
			DrawColour("Tab Hovered", EditorSettings->TabHovered, OnStyleChanged);
			DrawColour("Tab Active", EditorSettings->TabActive, OnStyleChanged);
			DrawColour("Tab Unfocused", EditorSettings->TabUnfocused, OnStyleChanged);
			DrawColour("Tab Unfocused Active", EditorSettings->TabUnfocusedActive, OnStyleChanged);

			DrawColour("Tab Selected", EditorSettings->TabSelected, OnStyleChanged);
			DrawColour("Tab Dimmed", EditorSettings->TabDimmed, OnStyleChanged);
			DrawColour("Tab Dimmed Selected", EditorSettings->TabDimmedSelected, OnStyleChanged);
			DrawColour("Tab Dimmed Selected Overline", EditorSettings->TabDimmedSelectedOverline, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Title", Flags))
		{
			DrawColour("Title", EditorSettings->Title, OnStyleChanged);
			DrawColour("Title Active", EditorSettings->TitleActive, OnStyleChanged);
			DrawColour("Title Collapsed", EditorSettings->TitleCollapsed, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Text", Flags))
		{
			DrawColour("Text", EditorSettings->Text, OnStyleChanged);
			DrawColour("Text Disabled", EditorSettings->TextDisabled, OnStyleChanged);
			DrawColour("Text Link", EditorSettings->TextLink, OnStyleChanged);
			DrawColour("Text Selected Background", EditorSettings->TextSelectedBackground, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Scrollbar", Flags))
		{
			DrawColour("Scrollbar Background", EditorSettings->ScrollbarBackground, OnStyleChanged);
			DrawColour("Scrollbar Grab", EditorSettings->ScrollbarGrab, OnStyleChanged);
			DrawColour("Scrollbar Grab Hovered", EditorSettings->ScrollbarGrabHovered, OnStyleChanged);
			DrawColour("Scrollbar Grab Active", EditorSettings->ScrollbarGrabActive, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Misc", Flags))
		{
			DrawColour("Check Mark", EditorSettings->CheckMark, OnStyleChanged);
			DrawColour("Slider Grab", EditorSettings->SliderGrab, OnStyleChanged);
			DrawColour("Slider Grab Active", EditorSettings->SliderGrabActive, OnStyleChanged);
			DrawColour("Input Text Cursor", EditorSettings->InputTextCursor, OnStyleChanged);

			DrawColour("Tree Lines", EditorSettings->TreeLines, OnStyleChanged);
			DrawColour("Drag Drop Target", EditorSettings->DragDropTarget, OnStyleChanged);
			DrawColour("Drag Drop Target Background", EditorSettings->DragDropTargetBackground, OnStyleChanged);
			DrawColour("Unsaved Marker", EditorSettings->UnsavedMarker, OnStyleChanged);

			DrawColour("Modal Window Dim Background", EditorSettings->ModalWindowDimBackground, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Separator", Flags))
		{
			DrawColour("Separator", EditorSettings->Separator, OnStyleChanged);
			DrawColour("Separator Hovered", EditorSettings->SeparatorHovered, OnStyleChanged);
			DrawColour("Separator Active", EditorSettings->SeparatorActive, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Resize Grip", Flags))
		{
			DrawColour("Resize Grip", EditorSettings->ResizeGrip, OnStyleChanged);
			DrawColour("Resize Grip Hovered", EditorSettings->ResizeGripHovered, OnStyleChanged);
			DrawColour("Resize Grip Active", EditorSettings->ResizeGripActive, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Docking", Flags))
		{
			DrawColour("Docking Preview", EditorSettings->DockingPreview, OnStyleChanged);
			DrawColour("Docking Empty Background", EditorSettings->DockingEmptyBackground, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Plot", Flags))
		{
			DrawColour("Plot Lines", EditorSettings->PlotLines, OnStyleChanged);
			DrawColour("Plot Lines Hovered", EditorSettings->PlotLinesHovered, OnStyleChanged);
			DrawColour("Plot Histogram", EditorSettings->PlotHistogram, OnStyleChanged);
			DrawColour("Plot Histogram Hovered", EditorSettings->PlotHistogramHovered, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Table", Flags))
		{
			DrawColour("Table Header Background", EditorSettings->TableHeaderBackground, OnStyleChanged);
			DrawColour("Table Border Strong", EditorSettings->TableBorderStrong, OnStyleChanged);
			DrawColour("Table Border Light", EditorSettings->TableBorderLight, OnStyleChanged);
			DrawColour("Table Row Background", EditorSettings->TableRowBackground, OnStyleChanged);
			DrawColour("Table Row Background Alt", EditorSettings->TableRowBackgroundAlt, OnStyleChanged);
		}

		if (ImGui::CollapsingHeader("Navigation", Flags))
		{
			DrawColour("Nav Windowing Highlight", EditorSettings->NavWindowingHighlight, OnStyleChanged);
			DrawColour("Nav Windowing Dim Background", EditorSettings->NavWindowingDimBackground, OnStyleChanged);
			DrawColour("Nav Cursor", EditorSettings->NavCursor, OnStyleChanged);
		}
	}
}
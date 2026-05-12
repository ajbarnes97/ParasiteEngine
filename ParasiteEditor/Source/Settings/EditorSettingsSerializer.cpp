#include "pepch.h"
#include "EditorSettingsSerializer.h"

#include "fstream"
#include "yaml-cpp/yaml.h"


namespace YAML
{
	template<>
	struct convert<Parasite::SColour>
	{
		static Node encode(const Parasite::SColour& InColour)
		{
			Node Node;
			Node.push_back(InColour.R);
			Node.push_back(InColour.G);
			Node.push_back(InColour.B);
			Node.push_back(InColour.A);

			return Node;
		}

		static bool decode(const Node& InNode, Parasite::SColour& OutColour)
		{
			if (!InNode.IsSequence() || InNode.size() != 4)
			{
				return false;
			}

			OutColour.R = InNode[0].as<float>();
			OutColour.G = InNode[1].as<float>();
			OutColour.B = InNode[2].as<float>();
			OutColour.A = InNode[3].as<float>();

			return true;
		}
	};
}


namespace Parasite
{
	static YAML::Emitter& operator<<(YAML::Emitter& Out, const SColour& InColour)
	{
		Out << YAML::Flow;
		Out << YAML::BeginSeq << InColour.R << InColour.G << InColour.B << InColour.A << YAML::EndSeq;
		return Out;
	}

	void CEditorSettingsSerializer::Save(const std::string& InFilepath, const SEditorSettings& Settings)
	{
		YAML::Emitter Out;
		Out << YAML::BeginMap;

		Out << YAML::Key << "EditorSettings";
		Out << YAML::BeginMap;

		// Main
		Out << YAML::Key << "Background" << YAML::Value << Settings.Background;
		Out << YAML::Key << "Border" << YAML::Value << Settings.Border;

		// Window / Background
		Out << YAML::Key << "ChildBackground" << YAML::Value << Settings.ChildBackground;
		Out << YAML::Key << "PopupBackground" << YAML::Value << Settings.PopupBackground;
		Out << YAML::Key << "BorderShadow" << YAML::Value << Settings.BorderShadow;
		Out << YAML::Key << "MenuBarBackground" << YAML::Value << Settings.MenuBarBackground;

		// Headers
		Out << YAML::Key << "Header" << YAML::Value << Settings.Header;
		Out << YAML::Key << "HeaderHovered" << YAML::Value << Settings.HeaderHovered;
		Out << YAML::Key << "HeaderActive" << YAML::Value << Settings.HeaderActive;

		// Buttons
		Out << YAML::Key << "Button" << YAML::Value << Settings.Button;
		Out << YAML::Key << "ButtonHovered" << YAML::Value << Settings.ButtonHovered;
		Out << YAML::Key << "ButtonActive" << YAML::Value << Settings.ButtonActive;

		// Frame
		Out << YAML::Key << "FrameBackground" << YAML::Value << Settings.FrameBackground;
		Out << YAML::Key << "FrameHovered" << YAML::Value << Settings.FrameHovered;
		Out << YAML::Key << "FrameActive" << YAML::Value << Settings.FrameActive;

		// Tabs
		Out << YAML::Key << "Tab" << YAML::Value << Settings.Tab;
		Out << YAML::Key << "TabHovered" << YAML::Value << Settings.TabHovered;
		Out << YAML::Key << "TabActive" << YAML::Value << Settings.TabActive;
		Out << YAML::Key << "TabUnfocused" << YAML::Value << Settings.TabUnfocused;
		Out << YAML::Key << "TabUnfocusedActive" << YAML::Value << Settings.TabUnfocusedActive;

		Out << YAML::Key << "TabSelected" << YAML::Value << Settings.TabSelected;
		Out << YAML::Key << "TabDimmed" << YAML::Value << Settings.TabDimmed;
		Out << YAML::Key << "TabDimmedSelected" << YAML::Value << Settings.TabDimmedSelected;
		Out << YAML::Key << "TabDimmedSelectedOverline" << YAML::Value << Settings.TabDimmedSelectedOverline;

		// Title
		Out << YAML::Key << "Title" << YAML::Value << Settings.Title;
		Out << YAML::Key << "TitleActive" << YAML::Value << Settings.TitleActive;
		Out << YAML::Key << "TitleCollapsed" << YAML::Value << Settings.TitleCollapsed;

		// Text
		Out << YAML::Key << "Text" << YAML::Value << Settings.Text;
		Out << YAML::Key << "TextDisabled" << YAML::Value << Settings.TextDisabled;
		Out << YAML::Key << "TextLink" << YAML::Value << Settings.TextLink;
		Out << YAML::Key << "TextSelectedBackground" << YAML::Value << Settings.TextSelectedBackground;

		// Scrollbar
		Out << YAML::Key << "ScrollbarBackground" << YAML::Value << Settings.ScrollbarBackground;
		Out << YAML::Key << "ScrollbarGrab" << YAML::Value << Settings.ScrollbarGrab;
		Out << YAML::Key << "ScrollbarGrabHovered" << YAML::Value << Settings.ScrollbarGrabHovered;
		Out << YAML::Key << "ScrollbarGrabActive" << YAML::Value << Settings.ScrollbarGrabActive;

		// Misc
		Out << YAML::Key << "CheckMark" << YAML::Value << Settings.CheckMark;
		Out << YAML::Key << "SliderGrab" << YAML::Value << Settings.SliderGrab;
		Out << YAML::Key << "SliderGrabActive" << YAML::Value << Settings.SliderGrabActive;
		Out << YAML::Key << "InputTextCursor" << YAML::Value << Settings.InputTextCursor;

		Out << YAML::Key << "TreeLines" << YAML::Value << Settings.TreeLines;
		Out << YAML::Key << "DragDropTarget" << YAML::Value << Settings.DragDropTarget;
		Out << YAML::Key << "DragDropTargetBackground" << YAML::Value << Settings.DragDropTargetBackground;
		Out << YAML::Key << "UnsavedMarker" << YAML::Value << Settings.UnsavedMarker;

		Out << YAML::Key << "ModalWindowDimBackground" << YAML::Value << Settings.ModalWindowDimBackground;

		// Separator
		Out << YAML::Key << "Separator" << YAML::Value << Settings.Separator;
		Out << YAML::Key << "SeparatorHovered" << YAML::Value << Settings.SeparatorHovered;
		Out << YAML::Key << "SeparatorActive" << YAML::Value << Settings.SeparatorActive;

		// Resize Grip
		Out << YAML::Key << "ResizeGrip" << YAML::Value << Settings.ResizeGrip;
		Out << YAML::Key << "ResizeGripHovered" << YAML::Value << Settings.ResizeGripHovered;
		Out << YAML::Key << "ResizeGripActive" << YAML::Value << Settings.ResizeGripActive;

		// Docking
		Out << YAML::Key << "DockingPreview" << YAML::Value << Settings.DockingPreview;
		Out << YAML::Key << "DockingEmptyBackground" << YAML::Value << Settings.DockingEmptyBackground;

		// Plot
		Out << YAML::Key << "PlotLines" << YAML::Value << Settings.PlotLines;
		Out << YAML::Key << "PlotLinesHovered" << YAML::Value << Settings.PlotLinesHovered;
		Out << YAML::Key << "PlotHistogram" << YAML::Value << Settings.PlotHistogram;
		Out << YAML::Key << "PlotHistogramHovered" << YAML::Value << Settings.PlotHistogramHovered;

		// Table
		Out << YAML::Key << "TableHeaderBackground" << YAML::Value << Settings.TableHeaderBackground;
		Out << YAML::Key << "TableBorderStrong" << YAML::Value << Settings.TableBorderStrong;
		Out << YAML::Key << "TableBorderLight" << YAML::Value << Settings.TableBorderLight;
		Out << YAML::Key << "TableRowBackground" << YAML::Value << Settings.TableRowBackground;
		Out << YAML::Key << "TableRowBackgroundAlt" << YAML::Value << Settings.TableRowBackgroundAlt;

		// Navigation
		Out << YAML::Key << "NavWindowingHighlight" << YAML::Value << Settings.NavWindowingHighlight;
		Out << YAML::Key << "NavWindowingDimBackground" << YAML::Value << Settings.NavWindowingDimBackground;
		Out << YAML::Key << "NavCursor" << YAML::Value << Settings.NavCursor;

		Out << YAML::EndMap;
		Out << YAML::EndMap;

		std::ofstream Stream(InFilepath);
		Stream << Out.c_str();

		PE_CORE_LOG("Finished saving editor settings.");
	}

	bool CEditorSettingsSerializer::Load(const std::string& InFilepath, SEditorSettings& OutSettings)
	{
		std::ifstream Stream(InFilepath);
		YAML::Node Data = YAML::Load(Stream);

		if (!Data["EditorSettings"])
		{
			return false;
		}

		auto Settings = Data["EditorSettings"];

		// Main
		OutSettings.Background = Settings["Background"].as<SColour>();
		OutSettings.Border = Settings["Border"].as<SColour>();

		// Window / Background
		OutSettings.ChildBackground = Settings["ChildBackground"].as<SColour>();
		OutSettings.PopupBackground = Settings["PopupBackground"].as<SColour>();
		OutSettings.BorderShadow = Settings["BorderShadow"].as<SColour>();
		OutSettings.MenuBarBackground = Settings["MenuBarBackground"].as<SColour>();

		// Headers
		OutSettings.Header = Settings["Header"].as<SColour>();
		OutSettings.HeaderHovered = Settings["HeaderHovered"].as<SColour>();
		OutSettings.HeaderActive = Settings["HeaderActive"].as<SColour>();

		// Buttons
		OutSettings.Button = Settings["Button"].as<SColour>();
		OutSettings.ButtonHovered = Settings["ButtonHovered"].as<SColour>();
		OutSettings.ButtonActive = Settings["ButtonActive"].as<SColour>();

		// Frame
		OutSettings.FrameBackground = Settings["FrameBackground"].as<SColour>();
		OutSettings.FrameHovered = Settings["FrameHovered"].as<SColour>();
		OutSettings.FrameActive = Settings["FrameActive"].as<SColour>();

		// Tabs
		OutSettings.Tab = Settings["Tab"].as<SColour>();
		OutSettings.TabHovered = Settings["TabHovered"].as<SColour>();
		OutSettings.TabActive = Settings["TabActive"].as<SColour>();
		OutSettings.TabUnfocused = Settings["TabUnfocused"].as<SColour>();
		OutSettings.TabUnfocusedActive = Settings["TabUnfocusedActive"].as<SColour>();

		OutSettings.TabSelected = Settings["TabSelected"].as<SColour>();
		OutSettings.TabDimmed = Settings["TabDimmed"].as<SColour>();
		OutSettings.TabDimmedSelected = Settings["TabDimmedSelected"].as<SColour>();
		OutSettings.TabDimmedSelectedOverline = Settings["TabDimmedSelectedOverline"].as<SColour>();

		// Title
		OutSettings.Title = Settings["Title"].as<SColour>();
		OutSettings.TitleActive = Settings["TitleActive"].as<SColour>();
		OutSettings.TitleCollapsed = Settings["TitleCollapsed"].as<SColour>();

		// Text
		OutSettings.Text = Settings["Text"].as<SColour>();
		OutSettings.TextDisabled = Settings["TextDisabled"].as<SColour>();
		OutSettings.TextLink = Settings["TextLink"].as<SColour>();
		OutSettings.TextSelectedBackground = Settings["TextSelectedBackground"].as<SColour>();

		// Scrollbar
		OutSettings.ScrollbarBackground = Settings["ScrollbarBackground"].as<SColour>();
		OutSettings.ScrollbarGrab = Settings["ScrollbarGrab"].as<SColour>();
		OutSettings.ScrollbarGrabHovered = Settings["ScrollbarGrabHovered"].as<SColour>();
		OutSettings.ScrollbarGrabActive = Settings["ScrollbarGrabActive"].as<SColour>();

		// Misc
		OutSettings.CheckMark = Settings["CheckMark"].as<SColour>();
		OutSettings.SliderGrab = Settings["SliderGrab"].as<SColour>();
		OutSettings.SliderGrabActive = Settings["SliderGrabActive"].as<SColour>();
		OutSettings.InputTextCursor = Settings["InputTextCursor"].as<SColour>();

		OutSettings.TreeLines = Settings["TreeLines"].as<SColour>();
		OutSettings.DragDropTarget = Settings["DragDropTarget"].as<SColour>();
		OutSettings.DragDropTargetBackground = Settings["DragDropTargetBackground"].as<SColour>();
		OutSettings.UnsavedMarker = Settings["UnsavedMarker"].as<SColour>();

		OutSettings.ModalWindowDimBackground = Settings["ModalWindowDimBackground"].as<SColour>();

		// Separator
		OutSettings.Separator = Settings["Separator"].as<SColour>();
		OutSettings.SeparatorHovered = Settings["SeparatorHovered"].as<SColour>();
		OutSettings.SeparatorActive = Settings["SeparatorActive"].as<SColour>();

		// Resize Grip
		OutSettings.ResizeGrip = Settings["ResizeGrip"].as<SColour>();
		OutSettings.ResizeGripHovered = Settings["ResizeGripHovered"].as<SColour>();
		OutSettings.ResizeGripActive = Settings["ResizeGripActive"].as<SColour>();

		// Docking
		OutSettings.DockingPreview = Settings["DockingPreview"].as<SColour>();
		OutSettings.DockingEmptyBackground = Settings["DockingEmptyBackground"].as<SColour>();

		// Plot
		OutSettings.PlotLines = Settings["PlotLines"].as<SColour>();
		OutSettings.PlotLinesHovered = Settings["PlotLinesHovered"].as<SColour>();
		OutSettings.PlotHistogram = Settings["PlotHistogram"].as<SColour>();
		OutSettings.PlotHistogramHovered = Settings["PlotHistogramHovered"].as<SColour>();

		// Table
		OutSettings.TableHeaderBackground = Settings["TableHeaderBackground"].as<SColour>();
		OutSettings.TableBorderStrong = Settings["TableBorderStrong"].as<SColour>();
		OutSettings.TableBorderLight = Settings["TableBorderLight"].as<SColour>();
		OutSettings.TableRowBackground = Settings["TableRowBackground"].as<SColour>();
		OutSettings.TableRowBackgroundAlt = Settings["TableRowBackgroundAlt"].as<SColour>();

		// Navigation
		OutSettings.NavWindowingHighlight = Settings["NavWindowingHighlight"].as<SColour>();
		OutSettings.NavWindowingDimBackground = Settings["NavWindowingDimBackground"].as<SColour>();
		OutSettings.NavCursor = Settings["NavCursor"].as<SColour>();

		PE_CORE_LOG("Finished loading editor settings.");
		return true;
	}
}
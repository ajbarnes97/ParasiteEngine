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

		// Headers
		Out << YAML::Key << "Header" << YAML::Value << Settings.Header;
		Out << YAML::Key << "HeaderHover" << YAML::Value << Settings.HeaderHovered;
		Out << YAML::Key << "HeaderActive" << YAML::Value << Settings.HeaderActive;

		// Buttons
		Out << YAML::Key << "Button" << YAML::Value << Settings.Button;
		Out << YAML::Key << "ButtonHoverd" << YAML::Value << Settings.ButtonHovered;
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

		// Title
		Out << YAML::Key << "Title" << YAML::Value << Settings.Title;
		Out << YAML::Key << "TitleActive" << YAML::Value << Settings.TitleActive;
		Out << YAML::Key << "TitleCollapsed" << YAML::Value << Settings.TitleCollapsed;

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

		// Headers
		OutSettings.Header = Settings["Header"].as<SColour>();
		OutSettings.HeaderHovered = Settings["HeaderHover"].as<SColour>();
		OutSettings.HeaderActive = Settings["HeaderActive"].as<SColour>();

		// Buttons
		OutSettings.Button = Settings["Button"].as<SColour>();
		OutSettings.ButtonHovered = Settings["ButtonHoverd"].as<SColour>();
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

		// Title
		OutSettings.Title = Settings["Title"].as<SColour>();
		OutSettings.TitleActive = Settings["TitleActive"].as<SColour>();
		OutSettings.TitleCollapsed = Settings["TitleCollapsed"].as<SColour>();

		PE_CORE_LOG("Finished loading editor settings.");
		return true;
	}
}
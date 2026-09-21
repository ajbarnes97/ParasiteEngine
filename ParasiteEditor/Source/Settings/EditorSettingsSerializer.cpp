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

		Settings.GetClass()->ForEachProperty([&Out, &Settings](const SPropertyInfo& InProperty)
		{
			if (InProperty.Type == std::type_index(typeid(SColour)))
			{
				const SColour Value = std::any_cast<SColour>(InProperty.Get(&Settings));
				Out << YAML::Key << InProperty.Name << YAML::Value << Value;
			}
			else
			{
				PE_CORE_LOG("EditorSettingsSerializer: unsupported reflected property type for '{}'", InProperty.Name);
			}
		});

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
		OutSettings.GetClass()->ForEachProperty([&](const SPropertyInfo& InProperty)
		{
			if (!Settings[InProperty.Name])
			{
				return;
			}

			if (InProperty.Type == std::type_index(typeid(SColour)))
			{
				InProperty.Set(&OutSettings, std::any(Settings[InProperty.Name].as<SColour>()));
			}
			else
			{
				PE_CORE_LOG("EditorSettingsSerializer: unsupported reflected property type for '{}'", InProperty.Name);
			}
		});

		PE_CORE_LOG("Finished loading editor settings.");
		return true;
	}
}
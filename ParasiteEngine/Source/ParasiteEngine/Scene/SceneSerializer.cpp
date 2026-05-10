#include "pepch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Component.h"

#include "fstream"
#include "yaml-cpp/yaml.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& InVec)
		{
			Node YamlNode;
			YamlNode.push_back(InVec.x);
			YamlNode.push_back(InVec.y);
			YamlNode.push_back(InVec.z);
			return YamlNode;
		}

		static bool decode(const Node& InNode, glm::vec3& InVec)
		{
			if (!InNode.IsSequence() || InNode.size() != 3)
			{
				return false;
			}

			InVec.x = InNode[0].as<float>();
			InVec.y = InNode[1].as<float>();
			InVec.z = InNode[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& InVec)
		{
			Node YamlNode;
			YamlNode.push_back(InVec.x);
			YamlNode.push_back(InVec.y);
			YamlNode.push_back(InVec.z);
			YamlNode.push_back(InVec.w);
			return YamlNode;
		}

		static bool decode(const Node& InNode, glm::vec4& InVec)
		{
			if (!InNode.IsSequence() || InNode.size() != 4)
			{
				return false;
			}

			InVec.x = InNode[0].as<float>();
			InVec.y = InNode[1].as<float>();
			InVec.z = InNode[2].as<float>();
			InVec.w = InNode[3].as<float>();
			return true;
		}
	};
}


namespace Parasite
{
	YAML::Emitter& operator<<(YAML::Emitter& Out, const glm::vec3 InVec)
	{
		Out << YAML::Flow;
		Out << YAML::BeginSeq << InVec.x << InVec.y << InVec.z << YAML::EndSeq;
		return Out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& Out, const glm::vec4 InVec)
	{
		Out << YAML::Flow;
		Out << YAML::BeginSeq << InVec.x << InVec.y << InVec.x << InVec.w << YAML::EndSeq;
		return Out;
	}

	CSceneSerializer::CSceneSerializer(const TSharedPtr<CScene>& InScene)
		: Scene(InScene)
	{
	}

	static void SerializeEntity(YAML::Emitter& Out, CEntity InEntity)
	{
		Out << YAML::BeginMap;
		Out << YAML::Key << "Entity" << YAML::Value << "12837";	// todo: Setup entity ID's

		if (InEntity.HasComponent<STagComponent>())
		{
			Out << YAML::Key << "TagComponent";
			Out << YAML::BeginMap;

			auto& Tag = InEntity.GetComponent<STagComponent>().Tag;
			Out << YAML::Key << "Tag" << YAML::Value << Tag;

			Out << YAML::EndMap;
		}

		if (InEntity.HasComponent<STransformComponent>())
		{
			Out << YAML::Key << "TransformComponent";
			Out << YAML::BeginMap;

			auto& TransformComponent = InEntity.GetComponent<STransformComponent>();

			Out << YAML::Key << "Translation" << YAML::Value << TransformComponent.Translation;
			Out << YAML::Key << "Rotation" << YAML::Value << TransformComponent.Rotation;
			Out << YAML::Key << "Scale" << YAML::Value << TransformComponent.Scale;

			Out << YAML::EndMap;
		}

		if (InEntity.HasComponent<SCameraComponent>())
		{
			Out << YAML::Key << "CameraComponent";
			Out << YAML::BeginMap;

			auto& CameraComponent = InEntity.GetComponent<SCameraComponent>();
			auto& Camera = CameraComponent.Camera;

			Out << YAML::Key << "Camera" << YAML::Value;
			Out << YAML::BeginMap;
			Out << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(Camera.GetProjectionType());
			Out << YAML::Key << "PerspectiveFOV" << YAML::Value << Camera.GetPerspectiveFOV();
			Out << YAML::Key << "PerspectiveNearClip" << YAML::Value << Camera.GetPerspectiveNearClip();
			Out << YAML::Key << "PerspectiveFarClip" << YAML::Value << Camera.GetPerspectiveFarClip();
			Out << YAML::Key << "OrthographicSize" << YAML::Value << Camera.GetOrthographicSize();
			Out << YAML::Key << "OrthographicNearClip" << YAML::Value << Camera.GetOrthographicNearClip();
			Out << YAML::Key << "OrthographicFarClip" << YAML::Value << Camera.GetOrthographicFarClip();
			Out << YAML::EndMap; // Camera

			Out << YAML::Key << "Primary" << YAML::Value << CameraComponent.bPrimaryCamera;

			Out << YAML::EndMap; /// Camera component
		}

		if (InEntity.HasComponent<SSpriteRendererComponent>())
		{
			Out << YAML::Key << "SpriteRendererComponent";
			Out << YAML::BeginMap;

			auto& RendererComponent = InEntity.GetComponent<SSpriteRendererComponent>();

			Out << YAML::Key << "Colour" << YAML::Value << RendererComponent.Colour;

			Out << YAML::EndMap;
		}

		Out << YAML::EndMap; // Entity map
	}

	void CSceneSerializer::Serialize(const std::string& InFilepath)
	{
		YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "Scene" << YAML::Value<< "Name";
		Out << YAML::Key<< "Entities" << YAML::Value<< YAML::BeginSeq;
		Scene->Registry.view<entt::entity>().each([&](auto EntityID)
		{
			CEntity Entity(EntityID, Scene.get());

			SerializeEntity(Out, Entity);
		});
		Out << YAML::EndSeq;
		Out << YAML::EndMap;

		std::ofstream fout(InFilepath);
		fout << Out.c_str();
	}

	void CSceneSerializer::SerializeBinary(const std::string& InFilepath)
	{

	}

	void CSceneSerializer::Deserialize(const std::string& InFilepath)
	{
		std::ifstream Stream(InFilepath);
		std::stringstream StringStream;
		StringStream << Stream.rdbuf();

		YAML::Node Data = YAML::Load(StringStream.str());
		if (!Data["Scene"])
		{
			return;
		}

		std::string SceneName = Data["Scene"].as<std::string>();
		PE_CORE_LOG("Deserializing scene {0}", SceneName);

		auto Entities = Data["Entities"];
		if (Entities)
		{
			for (auto Entity : Entities)
			{
				uint64_t UUID = 0;// = Entity.as<uint64_t>();	// todo: setup entity ID's

				std::string Name;
				auto TagComponent = Entity["TagComponent"];
				if (TagComponent)
				{
					Name = TagComponent["Tag"].as<std::string>();
				}

				PE_CORE_LOG("Deserializing entity ({0}) with ID ({1})", Name, UUID);

				CEntity DeserializedEntity = Scene->CreateEntity(Name);
				
				if (auto Component = Entity["TransformComponent"])
				{
					auto& TransformComponent = DeserializedEntity.GetComponent<STransformComponent>();
					TransformComponent.Translation = Component["Translation"].as<glm::vec3>();
					TransformComponent.Rotation = Component["Rotation"].as<glm::vec3>();
					TransformComponent.Scale = Component["Scale"].as<glm::vec3>();
				}

				if (auto Component = Entity["CameraComponent"])
				{
					auto& CameraComponent = DeserializedEntity.AddComponent<SCameraComponent>();

					auto CameraProps = Component["Camera"];
					const CCamera::EProjectionType Type = static_cast<CCamera::EProjectionType>(CameraProps["ProjectionType"].as<int>());
					CameraComponent.Camera.SetProjectionType(Type);

					CameraComponent.Camera.SetPerspectiveFOV(CameraProps["PerspectiveFOV"].as<float>());
					CameraComponent.Camera.SetPerspectiveNearClip(CameraProps["PerspectiveNearClip"].as<float>());
					CameraComponent.Camera.SetPerspectiveFarClip(CameraProps["PerspectiveFarClip"].as<float>());

					CameraComponent.Camera.SetOrthographicSize(CameraProps["OrthographicSize"].as<float>());
					CameraComponent.Camera.SetOrthographicNearClip(CameraProps["OrthographicNearClip"].as<float>());
					CameraComponent.Camera.SetOrthographicFarClip(CameraProps["OrthographicFarClip"].as<float>());

					CameraComponent.bPrimaryCamera = Component["Primary"].as<bool>();
				}

				if (auto Component = Entity["SpriteRendererComponent"])
				{
					auto& SpriteRendererComponent = DeserializedEntity.AddComponent<SSpriteRendererComponent>();
					SpriteRendererComponent.Colour = Component["Colour"].as<glm::vec4>();
				}
			}
		}
	}

	void CSceneSerializer::DeserializeBinary(const std::string& InFilepath)
	{

	}
}
#include "pepch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "ParasiteEngine/Renderer/Renderer2D.h"

#include "../entt/entt.hpp"


namespace Parasite
{
	CScene::CScene()
	{
	}

	CScene::~CScene()
	{

	}

	CEntity CScene::CreateEntity(const std::string& InName)
	{
		CEntity NewEntity = CEntity(Registry.create(), this);
		NewEntity.AddComponent<STransformComponent>();
		STagComponent& TagComponent = NewEntity.AddComponent<STagComponent>();
		TagComponent.Tag = InName.empty() ? "Entity" : InName;

		return NewEntity;
	}

	void CScene::OnUpdate(CTimestep InTimestep)
	{
		// Update scripts
		{
			Registry.view<SNativeScriptComponent>().each([=](auto InEntity, auto& InScriptComponent)
			{
				// todo: this will need to be moved to runtime start when setup
				if (!InScriptComponent.Instance)
				{
					InScriptComponent.Instance = InScriptComponent.InstantiateScript();
					InScriptComponent.Instance->Entity = CEntity(InEntity, this);
					InScriptComponent.Instance->OnCreate();
				}

				InScriptComponent.Instance->OnUpdate(InTimestep);
			});
		}

		CCamera* PrimaryCamera = nullptr;
		glm::mat4 CameraTransform;
		{
			auto Group = Registry.view<SCameraComponent, STransformComponent>();
			for (auto Entity : Group)
			{
				auto Transform = Group.get<STransformComponent>(Entity);
				auto Camera = Group.get<SCameraComponent>(Entity);
				if (Camera.bPrimaryCamera)
				{
					PrimaryCamera = &Camera.Camera;
					CameraTransform = Transform.GetTransform();
					break;
				}
			}
		}

		if (PrimaryCamera)
		{
			CRenderer2D::BeginScene(*PrimaryCamera, CameraTransform);

			auto Group = Registry.group<STransformComponent, SSpriteRendererComponent>();
			for (auto Entity : Group)
			{
				auto Transform = Group.get<STransformComponent>(Entity);
				auto Sprite = Group.get<SSpriteRendererComponent>(Entity);

				CRenderer2D::DrawQuad(Transform.GetTransform(), Sprite.Colour);
			}

			CRenderer2D::EndScene();
		}
	}
}
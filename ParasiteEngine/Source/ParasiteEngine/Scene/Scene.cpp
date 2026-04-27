#include "pepch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"


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

	}
}
#pragma once
#include "ParasiteEngine/Core/Timestep.h"

#include "../entt/entt.hpp"
#include "../Renderer/EditorCamera.h"


namespace Parasite
{
	class CEntity;

	class CScene
	{
	public:
		CScene();
		~CScene();

		CEntity CreateEntity(const std::string& InName = std::string());
		void DestroyEntity(CEntity InEntity);

		void OnUpdateEditor(CTimestep InTimestep, CEditorCamera& InCamera);
		void OnUpdateRuntime(CTimestep InTimestep);

		CEntity GetPrimaryCameraEntity();

	private:
		entt::registry Registry;
		
		friend class CEntity;
		friend class CSceneHierarchyPanel;
		friend class CSceneSerializer;
	};
}
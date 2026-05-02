#pragma once
#include "ParasiteEngine/Core/Timestep.h"

#include "../entt/entt.hpp"


namespace Parasite
{
	class CEntity;

	class CScene
	{
	public:
		CScene();
		~CScene();

		CEntity CreateEntity(const std::string& InName = std::string());

		void OnUpdate(CTimestep InTimestep);

	private:
		entt::registry Registry;
		
		friend class CEntity;
		friend class CSceneHierarchyPanel;
	};
}
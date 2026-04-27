#include "pepch.h"
#include "ParasiteEngine/Scene/Entity.h"

namespace Parasite
{
	CEntity::CEntity(entt::entity InEntityHandle, CScene* InScene) 
		: EntityHandle(InEntityHandle),
			Scene(InScene)
	{
	}
}
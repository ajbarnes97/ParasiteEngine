#pragma once
#include "ParasiteEngine/Scene/Scene.h"
#include "ParasiteEngine/Core/Core.h"

#include "../entt/entt.hpp"


namespace Parasite
{
	class CEntity
	{
	public:
		CEntity() = default;
		CEntity(entt::entity InEntityHandle, CScene* InScene);
		CEntity(const CEntity&) = default;

		operator const bool() const { return EntityHandle != entt::null; }
		operator const uint32_t () const { return static_cast<uint32_t>(EntityHandle); }
		explicit operator const uint64_t() const { return static_cast<uint64_t>(EntityHandle); }
		bool operator ==(const CEntity& InOther) const 
		{ 
			return EntityHandle == InOther.EntityHandle && Scene == InOther.Scene; 
		}
		bool operator !=(const CEntity& InOther) const { return !operator==(InOther); }

		template<typename T>
		bool HasComponent();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... InArgs);

		template<typename T>
		void RemoveComponent();

		template<typename T>
		T& GetComponent();

	private:
		entt::entity EntityHandle = entt::null;
		CScene* Scene = nullptr;
	};


	template<typename T>
	bool CEntity::HasComponent()
	{
		return Scene->Registry.any_of<T>(EntityHandle);
	}

	template<typename T, typename... Args>
	T& CEntity::AddComponent(Args&&... InArgs)
	{
		PE_CORE_ASSERT(!HasComponent<T>(), "Attempting to add a component of an already existing type.");
		return Scene->Registry.emplace<T>(EntityHandle, std::forward<Args>(InArgs)...);
	}

	template<typename T>
	T& CEntity::GetComponent()
	{
		PE_CORE_ASSERT(HasComponent<T>(), "Entity has no component of type.");
		return Scene->Registry.get<T>(EntityHandle);
	}

	template<typename T>
	void CEntity::RemoveComponent()
	{
		PE_CORE_ASSERT(HasComponent<T>(), "Entity has no component of type.");
		Scene->Registry.remove(EntityHandle);
	}
}
#include "SceneHierarchyPanel.h"

#include "ParasiteEngine/Scene/Component.h"

#include "ImGui/imgui.h"


namespace Parasite
{
	CSceneHierarchyPanel::CSceneHierarchyPanel(const TSharedPtr<CScene>& InScene)
	{
		SetContext(InScene);
	}

	void CSceneHierarchyPanel::SetContext(const TSharedPtr<CScene>& InScene)
	{
		Context = InScene;
	}

	void CSceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		Context->Registry.view<entt::entity>().each([this](auto InEntityID)
			{
				CEntity Entity { InEntityID, Context.get() };
				DrawEntityNode(Entity);
			}
		);

		ImGui::End();
	}

	void CSceneHierarchyPanel::DrawEntityNode(CEntity InEntity)
	{
		auto& Tag = InEntity.GetComponent<STagComponent>().Tag;

		ImGuiTreeNodeFlags Flags = ((SelectionContext == InEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		const bool bOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(InEntity)), Flags, Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectionContext = InEntity;
		}

		if (bOpened)
		{
			ImGui::TreePop();
		}
	}
}
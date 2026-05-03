#include "SceneHierarchyPanel.h"

#include "ParasiteEngine/Scene/Component.h"

#include "ImGui/imgui.h"
#include "glm/gtc/type_ptr.hpp"


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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Details");
		if (SelectionContext)
		{
			DrawComponents(SelectionContext);
		}
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

	void CSceneHierarchyPanel::DrawComponents(CEntity InEntity)
	{
		auto& Tag = InEntity.GetComponent<STagComponent>().Tag;
		if (InEntity.HasComponent<STagComponent>())
		{
			char Buffer[256];
			memset(Buffer, 0, sizeof(Buffer));
			strcpy_s(Buffer, Tag.c_str());

			if (ImGui::InputText("Tag", Buffer, sizeof(Buffer)))
			{
				Tag = std::string(Buffer);
			}
		}
		
		if (InEntity.HasComponent<STransformComponent>())
		{
			void* ID = reinterpret_cast<void*>(typeid(STransformComponent).hash_code());
			if (ImGui::TreeNodeEx(ID, ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& Transform = InEntity.GetComponent<STransformComponent>().Transform;
				ImGui::DragFloat3("Location", glm::value_ptr(Transform[3]), 0.05f);
				
				ImGui::TreePop();
			}
		}

		if (InEntity.HasComponent<SCameraComponent>())
		{
			void* ID = reinterpret_cast<void*>(typeid(SCameraComponent).hash_code());
			if (ImGui::TreeNodeEx(ID, ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& CameraComponent = InEntity.GetComponent<SCameraComponent>();
				auto& Camera = CameraComponent.Camera;

				const char* ProjectionTypeStrings[] = { "Projection", "Orthographic" };
				const char* CurrentProjectionTypeString = ProjectionTypeStrings[static_cast<int>(Camera.GetProjectionType())];

				if (ImGui::BeginCombo("Projection", CurrentProjectionTypeString))
				{
					for (int Index = 0; Index < 2; Index++)
					{
						const bool bIsSelected = CurrentProjectionTypeString == ProjectionTypeStrings[Index];
						if (ImGui::Selectable(ProjectionTypeStrings[Index], bIsSelected))
						{
							CurrentProjectionTypeString = ProjectionTypeStrings[Index];
							Camera.SetProjectionType(static_cast<CCamera::EProjectionType>(Index));
						}

						if (bIsSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (Camera.GetProjectionType() == CCamera::EProjectionType::Perspective)
				{
					float FieldOfView = Camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("FOV", &FieldOfView))
					{
						Camera.SetPerspectiveFOV(FieldOfView);
					}

					float PerspectiveNearClip = Camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &PerspectiveNearClip))
					{
						Camera.SetPerspectiveNearClip(PerspectiveNearClip);
					}

					float PerspectiveFarClip = Camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &PerspectiveFarClip))
					{
						Camera.SetPerspectiveFarClip(PerspectiveFarClip);
					}
				}
				else
				{
					float Size = Camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &Size))
					{
						Camera.SetOrthographicSize(Size);
					}

					float OrthographicNearClip = Camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &OrthographicNearClip))
					{
						Camera.SetOrthographicNearClip(OrthographicNearClip);
					}

					float OrthographicFarClip = Camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &OrthographicFarClip))
					{
						Camera.SetOrthographicFarClip(OrthographicFarClip);
					}
				}

				ImGui::TreePop();
			}
		}
	}
}
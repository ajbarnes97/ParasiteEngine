#include "SceneHierarchyPanel.h"

#include "ParasiteEngine/Scene/Component.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
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

	static void DrawVec3Control(const std::string& InLabel, glm::vec3& InValue, float InResetValue = 0.0f, float InColumnWidth = 100.0f)
	{
		ImGui::PushID(InLabel.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, InColumnWidth);
		ImGui::Text(InLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float LineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 ButtonSize = { LineHeight + 3.0f, LineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
		if (ImGui::Button("X", ButtonSize))
		{
			InValue.x = InResetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &InValue.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));
		if (ImGui::Button("Y", ButtonSize))
		{
			InValue.y = InResetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &InValue.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.8f, 1.0f));
		if (ImGui::Button("Z", ButtonSize))
		{
			InValue.z = InResetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &InValue.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
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
				auto& TransformComponent = InEntity.GetComponent<STransformComponent>();

				DrawVec3Control("Translation", TransformComponent.Translation);
				glm::vec3 Rotation = glm::degrees(TransformComponent.Rotation);
				DrawVec3Control("Rotation", Rotation);
				TransformComponent.Rotation = glm::radians(Rotation);
				DrawVec3Control("Scale", TransformComponent.Scale, 1.0f);

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
	
		if (InEntity.HasComponent<SSpriteRendererComponent>())
		{
			void* ID = reinterpret_cast<void*>(typeid(SSpriteRendererComponent).hash_code());
			if (ImGui::TreeNodeEx(ID, ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& RendererComponent = InEntity.GetComponent<SSpriteRendererComponent>();
				ImGui::ColorEdit4("Colour", glm::value_ptr(RendererComponent.Colour));
				ImGui::TreePop();
			}
		}
	}
}
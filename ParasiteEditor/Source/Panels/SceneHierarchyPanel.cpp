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
		SelectionContext = CEntity::NullEntity();
	}

	void CSceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (!Context)
		{
			ImGui::End();
			return;
		}

		Context->Registry.view<entt::entity>().each([this](auto InEntityID)
		{
			CEntity Entity{ InEntityID, Context.get() };
			DrawEntityNode(Entity);
		}
		);

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			SelectionContext = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				Context->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Details");
		if (SelectionContext)
		{
			DrawComponents(SelectionContext);
		}
		ImGui::End();
	}

	void CSceneHierarchyPanel::SetSelectedContext(CEntity InEntity)
	{
		SelectionContext = InEntity;
	}

	void CSceneHierarchyPanel::DrawEntityNode(CEntity InEntity)
	{
		auto& Tag = InEntity.GetComponent<STagComponent>().Tag;

		ImGuiTreeNodeFlags Flags = ((SelectionContext == InEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		Flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		const bool bOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(InEntity)), Flags, Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			SelectionContext = InEntity;
		}

		bool bEntityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				bEntityDeleted = true;
				if (InEntity == SelectionContext)
				{
					SelectionContext = CEntity::NullEntity();
				}
			}

			ImGui::EndPopup();
		}

		if (bOpened)
		{
			ImGui::TreePop();
		}

		if (bEntityDeleted)
		{
			Context->DestroyEntity(InEntity);
		}
	}

	static void DrawVec3Control(const std::string& InLabel, glm::vec3& InValue, float InResetValue = 0.0f, float InColumnWidth = 100.0f)
	{
		ImGuiIO& IO = ImGui::GetIO();
		auto& BoldFont = IO.Fonts->Fonts[0];

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

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X", ButtonSize))
		{
			InValue.x = InResetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &InValue.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y", ButtonSize))
		{
			InValue.y = InResetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &InValue.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.8f, 1.0f));

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z", ButtonSize))
		{
			InValue.z = InResetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &InValue.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename Func>
	static void DrawComponent(const std::string& InLabel, CEntity InEntity, Func InFunc)
	{
		if (InEntity.HasComponent<T>())
		{
			auto& Component = InEntity.GetComponent<T>();
			const ImVec2 ContentRegionAvailable = ImGui::GetContentRegionAvail();

			const ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap | 
					ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			const float LineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y + 2.0f;
			ImGui::Separator();
			void* ID = reinterpret_cast<void*>(typeid(T).hash_code());
			bool bOpen = (ImGui::TreeNodeEx(ID, TreeNodeFlags, InLabel.c_str()));
			ImGui::PopStyleVar();

			ImGui::SameLine(ContentRegionAvailable.x - (LineHeight * 0.5f));
			if (ImGui::Button("...", ImVec2(LineHeight, LineHeight)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool bRemoveComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					bRemoveComponent = true;
				}
				ImGui::EndPopup();
			}

			if (bOpen)
			{
				InFunc(Component);
				ImGui::TreePop();
			}

			if (bRemoveComponent)
			{
				InEntity.RemoveComponent<T>();
			}
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

			if (ImGui::InputText("##Tag", Buffer, sizeof(Buffer)))
			{
				Tag = std::string(Buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!SelectionContext.HasComponent<SCameraComponent>() && ImGui::MenuItem("Camera"))
			{
				SelectionContext.AddComponent<SCameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (!SelectionContext.HasComponent<SSpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
			{
				SelectionContext.AddComponent<SSpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		DrawComponent<STransformComponent>("Transform Component", InEntity, [](auto& InComponent)
		{
			DrawVec3Control("Translation", InComponent.Translation);
			glm::vec3 Rotation = glm::degrees(InComponent.Rotation);
			DrawVec3Control("Rotation", Rotation);
			InComponent.Rotation = glm::radians(Rotation);
			DrawVec3Control("Scale", InComponent.Scale, 1.0f);
		});
	
		DrawComponent<SCameraComponent>("Camera Component", InEntity, [](auto& InComponent)
		{
			auto& Camera = InComponent.Camera;

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
		});

		DrawComponent<SSpriteRendererComponent>("Sprite Renderer Component", InEntity, [](auto& InComponent)
		{
			ImGui::ColorEdit4("Colour", glm::value_ptr(InComponent.Colour));
		});
	}
}
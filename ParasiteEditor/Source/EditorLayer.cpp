#include "EditorLayer.h"

#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"
#include "ParasiteEngine/Scene/SceneSerializer.h"
#include "ParasiteEngine/Utils/PlatformUtils.h"
#include "ParasiteEngine/Math/Math.h"
#include "ParasiteEngine/Math/Vector.h"

#include "Settings/EditorSettingsSerializer.h"


#include "ImGuizmo.h"
#include "ImGui/imgui.h"
#include "glm/gtc/type_ptr.hpp"


namespace Parasite
{
	CEditorLayer::CEditorLayer() : CLayer("Parasite Editor Layer")
		, Camera(1.7777f, true)
	{
	}

	void CEditorLayer::OnAttach()
	{
		if (!CEditorSettingsSerializer::Load(SettingsPath, EditorStyle))
		{
			PE_CORE_WARN("Failed to load editor settings from path: ({0}). Creating default.", SettingsPath);
			CEditorSettingsSerializer::Save(SettingsPath, EditorStyle);
		}

		EditorSettingsPanel.OnStyleChanged = (
			[this](const SColour& InColour)
			{
				ApplyEditorTheme();
			});
		EditorSettingsPanel.OnEditorSettingsApplied = (
			[this](const SEditorSettings* InSettings)
			{
				CEditorSettingsSerializer::Save(SettingsPath, EditorStyle);
			});

		ApplyEditorTheme();

		SFrameBufferSpecification Specification;
		Specification.Width = 1280;
		Specification.Height = 720;

		FrameBuffer = CFrameBuffer::Create(Specification);
	}

	void CEditorLayer::OnDetach()
	{
	}

	void CEditorLayer::OnUpdate(CTimestep InTimestep)
	{
		if (bViewportFocused)
		{
			Camera.OnUpdate(InTimestep);
		}

		FrameBuffer->Bind();

		CRenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		CRenderCommand::Clear();

		if (ActiveScene)
		{
			ActiveScene->OnUpdate(InTimestep);
		}
		FrameBuffer->Unbind();
	}

	void CEditorLayer::OnImGuiRender()
	{
		static bool bOpenDockspace = true;

		ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(Viewport->Pos);
		ImGui::SetNextWindowSize(Viewport->Size);
		ImGui::SetNextWindowViewport(Viewport->ID);

		WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("DockSpace", &bOpenDockspace, WindowFlags);
		ImGui::PopStyleVar(2);

		ImGuiStyle& Style = ImGui::GetStyle();

		const float MinWindowSize = Style.WindowMinSize.x;
		// Set min window size for docked windows
		Style.WindowMinSize.x = 370.0f; 

		ImGuiID DockspaceID = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
		Style.WindowMinSize.x = MinWindowSize;

		DrawMainMenuBar();

		EditorSettingsPanel.OnImGuiRender();
		HierarchyPanel.OnImGuiRender();

		DrawViewport();
		DrawGizmos();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void CEditorLayer::OnEvent(CEvent& InEvent)
	{
		Camera.OnEvent(InEvent);

		CEventDispatcher Dispatcher(InEvent);
		Dispatcher.Dispatch<CPressedKeyEvent>(PE_BIND_EVENT_FUNC(CEditorLayer::OnKeyPressed));
	}

	void CEditorLayer::DrawGizmos()
	{
		if (!ActiveScene)
		{
			return;
		}

		CEntity Entity = HierarchyPanel.GetSelectionContext();
		if (Entity && GizmoType >= 0)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float WindowWidth = static_cast<float>(ImGui::GetWindowWidth());
			float WindowHeight = static_cast<float>(ImGui::GetWindowHeight());
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight);
			
			auto CameraEntity = ActiveScene->GetPrimaryCameraEntity();
			const auto& Camera = CameraEntity.GetComponent<SCameraComponent>().Camera;
			const glm::mat4& CameraProjection = Camera.GetProjectionMatrix();
			glm::mat4 CameraView = glm::inverse(CameraEntity.GetComponent<STransformComponent>().GetTransform());

			auto& TransformComponent = Entity.GetComponent<STransformComponent>();
			glm::mat4 Transform = TransformComponent.GetTransform();

			// Snapping
			bool bShouldSnap = CInput::IsKeyPressed(PE_KEY_LEFT_CONTROL);
			float SnapValue = GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
			float SnapValues[3] = { SnapValue, SnapValue, SnapValue };
		
			ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), static_cast<ImGuizmo::OPERATION>(GizmoType), 
									ImGuizmo::LOCAL, glm::value_ptr(Transform), nullptr, bShouldSnap ? SnapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 Translation, Rotation, Scale;
				if (Math::DecomposeTransform(Transform, Translation, Rotation, Scale))
				{
					const glm::vec3 DeltaRotation = Rotation - TransformComponent.Rotation;

					TransformComponent.Translation = Translation;
					TransformComponent.Rotation += DeltaRotation;
					TransformComponent.Scale = Scale;
				}
			}
		}
	}

	bool CEditorLayer::OnKeyPressed(CPressedKeyEvent& InEvent)
	{
		if (InEvent.GetRepeatCount() > 0) 
		{
			return false;
		}

		const bool bIsControlPressed = CInput::IsKeyPressed(PE_KEY_LEFT_CONTROL) || CInput::IsKeyPressed(PE_KEY_RIGHT_CONTROL);
		const bool bIsShiftPressed = CInput::IsKeyPressed(PE_KEY_LEFT_SHIFT) || CInput::IsKeyPressed(PE_KEY_RIGHT_SHIFT);

		switch (InEvent.GetKeyCode())
		{
		case PE_KEY_Q:
		{
			GizmoType = INDEX_NONE;
			break;
		}
		case PE_KEY_W:
		{
			GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case PE_KEY_E:
		{
			GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case PE_KEY_R:
		{
			GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		case PE_KEY_N:
		{
			if (bIsControlPressed)
			{
				NewScene();
			}
			break;
		}
		case PE_KEY_O:
		{
			if (bIsControlPressed)
			{
				OpenScene();
			}
			break;
		}
		case PE_KEY_S:
		{
			if (bIsControlPressed)
			{
				if (bIsShiftPressed)
				{
					SaveSceneAs();
				}
				else
				{
					SaveScene();
				}
			}
			break;
		}
		}
		return false;
	}

	void CEditorLayer::NewScene()
	{
		ActiveScene = MakeShared<CScene>();
		HierarchyPanel.SetContext(ActiveScene);
	}

	void CEditorLayer::OpenScene()
	{
		std::string Filepath = CFileDialogs::OpenFile("Parasite Scene (*.pescene)\0*.pescene\0");
		if (!Filepath.empty())
		{
			ActiveScene = MakeShared<CScene>();
			HierarchyPanel.SetContext(ActiveScene);

			CSceneSerializer SceneSerializer(ActiveScene);
			SceneSerializer.Deserialize(Filepath);
			ActiveSceneFilePath = Filepath;
		}
	}

	void CEditorLayer::SaveScene()
	{
		PE_ASSERT(!ActiveSceneFilePath.empty(), "Failed to save scene.");
		if (!ActiveSceneFilePath.empty())
		{
			CSceneSerializer SceneSerializer(ActiveScene);
			SceneSerializer.Serialize(ActiveSceneFilePath);
		}
	}

	void CEditorLayer::SaveSceneAs()
	{
		std::string Filepath = CFileDialogs::SaveFile("Parasite Scene (*.pescene)\0*.pescene\0");
		if (!Filepath.empty())
		{
			CSceneSerializer SceneSerializer(ActiveScene);
			SceneSerializer.Serialize(Filepath);
		}
	}

	void CEditorLayer::DrawMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctr+N"))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Save", "Ctr+S"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Open", "Ctr+O"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save As", "Ctr+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit"))
				{
					CApplication::Get().Close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Editor Settings"))
				{
					EditorSettingsPanel.ShowWindow(&EditorStyle);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::End();
	}

	void CEditorLayer::DrawViewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		bViewportFocused = ImGui::IsWindowFocused();
		bViewportHovered = ImGui::IsWindowHovered();
		CApplication::Get().GetImGuiLayer()->SetBlockEvents(!bViewportFocused && !bViewportHovered);

		ImVec2 WindowSize = ImGui::GetContentRegionAvail();
		if (WindowSize.x != ViewportSize.x || WindowSize.y != ViewportSize.y)
		{
			ViewportSize = { WindowSize.x, WindowSize.y };
			FrameBuffer->Resize(static_cast<uint32_t>(ViewportSize.x), static_cast<uint32_t>(ViewportSize.y));

			Camera.ResizeBounds(ViewportSize.x, ViewportSize.y);
		}
		ImGui::Image((void*)(uintptr_t)FrameBuffer->GetColourAttachmentRendererID(), { ViewportSize.x, ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
	}

	void CEditorLayer::ApplyEditorTheme()
	{
		auto& StyleColours = ImGui::GetStyle().Colors;
		StyleColours[ImGuiCol_WindowBg] = ToImVec4(EditorStyle.Background);
		StyleColours[ImGuiCol_Border] = ToImVec4(EditorStyle.Border);

		// Headers
		StyleColours[ImGuiCol_Header] = ToImVec4(EditorStyle.Header);
		StyleColours[ImGuiCol_HeaderHovered] = ToImVec4(EditorStyle.HeaderHovered);
		StyleColours[ImGuiCol_HeaderActive] = ToImVec4(EditorStyle.HeaderActive);

		// Buttons
		StyleColours[ImGuiCol_Button] = ToImVec4(EditorStyle.Button);
		StyleColours[ImGuiCol_ButtonHovered] = ToImVec4(EditorStyle.ButtonHovered);
		StyleColours[ImGuiCol_ButtonActive] = ToImVec4(EditorStyle.ButtonActive);

		// Frame Background
		StyleColours[ImGuiCol_FrameBg] = ToImVec4(EditorStyle.FrameBackground);
		StyleColours[ImGuiCol_FrameBgHovered] = ToImVec4(EditorStyle.FrameHovered);
		StyleColours[ImGuiCol_FrameBgActive] = ToImVec4(EditorStyle.FrameActive);

		// Tabs
		StyleColours[ImGuiCol_Tab] = ToImVec4(EditorStyle.Tab);
		StyleColours[ImGuiCol_TabHovered] = ToImVec4(EditorStyle.TabHovered);
		StyleColours[ImGuiCol_TabActive] = ToImVec4(EditorStyle.TabActive);
		StyleColours[ImGuiCol_TabUnfocused] = ToImVec4(EditorStyle.TabUnfocused);
		StyleColours[ImGuiCol_TabUnfocusedActive] = ToImVec4(EditorStyle.TabUnfocusedActive);

		// Title
		StyleColours[ImGuiCol_TitleBg] = ToImVec4(EditorStyle.Title);
		StyleColours[ImGuiCol_TitleBgActive] = ToImVec4(EditorStyle.TitleActive);
		StyleColours[ImGuiCol_TitleBgCollapsed] = ToImVec4(EditorStyle.TitleCollapsed);
	}

	ImVec4 CEditorLayer::ToImVec4(const SColour& InColour)
	{
		return ImVec4(InColour.R, InColour.G, InColour.B, InColour.A);
	}
}
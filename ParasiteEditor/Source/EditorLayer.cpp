#include "EditorLayer.h"

#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"

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
		Texture = CTexture2D::Create("Assets/Textures/Checkerboard.png");
		SpriteSheet = CTexture2D::Create("Assets/Textures/TextureSheet.png");

		SubTexture = CSubTexture2D::CreateFromCoords(SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

		SFrameBufferSpecification Specification;
		Specification.Width = 1280;
		Specification.Height = 720;

		FrameBuffer = CFrameBuffer::Create(Specification);

		ActiveScene = MakeShared<CScene>();
		SqaureEntity = ActiveScene->CreateEntity("Sqaure");
		SqaureEntity.AddComponent<SSpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f });

		SqaureEntity2 = ActiveScene->CreateEntity("Sqaure2");
		SqaureEntity2.AddComponent<SSpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });

		CameraEntity = ActiveScene->CreateEntity("Camera");
		CameraEntity.AddComponent<SCameraComponent>();

		CameraTwoEntity = ActiveScene->CreateEntity("Camera Two");
		auto& CameraComp = CameraTwoEntity.AddComponent<SCameraComponent>();
		CameraComp.bPrimaryCamera = false;

		class CCameraController : public CScriptableEntity
		{
		public:
			void OnCreate()
			{
			}

			void OnUpdate(CTimestep InTimestep)
			{
				auto& Translation = GetComponent<STransformComponent>().Translation;
				float Speed = 5.0f;

				if (CInput::IsKeyPressed(PE_KEY_A))
					Translation.x -= Speed * InTimestep;
				if (CInput::IsKeyPressed(PE_KEY_D))
					Translation.x += Speed * InTimestep;
				if (CInput::IsKeyPressed(PE_KEY_W))
					Translation.y += Speed * InTimestep;
				if (CInput::IsKeyPressed(PE_KEY_S))
					Translation.y -= Speed * InTimestep;
			}

			void OnDestroy()
			{

			}
		};

	
		CameraTwoEntity.AddComponent<SNativeScriptComponent>().Bind<CCameraController>();
		CameraEntity.AddComponent<SNativeScriptComponent>().Bind<CCameraController>();

		HierarchyPanel.SetContext(ActiveScene);
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

		//CRenderer2D::BeginScene(Camera.GetCamera());
		//for (float y = -5.0f; y < 5.0f; y += 0.5f)
		//{
		//	for (float x = -5.0f; x < 5.0f; x += 0.5f)
		//	{
		//		glm::vec4 Colour = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
		//		CRenderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, Colour);
		//	}
		//}
		//CRenderer2D::EndScene();
		//
		//CRenderer2D::BeginScene(Camera.GetCamera());
		//CRenderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, SubTexture);
		//CRenderer2D::EndScene();
		ActiveScene->OnUpdate(InTimestep);

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

		ImGuiID DockspaceID = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					CApplication::Get().Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();

		HierarchyPanel.OnImGuiRender();

		ImGui::Begin("Camera Settings");
		if (ImGui::Checkbox("Camera A", &bPrimaryCamera))
		{
			CameraEntity.GetComponent<SCameraComponent>().bPrimaryCamera = bPrimaryCamera;
			CameraTwoEntity.GetComponent<SCameraComponent>().bPrimaryCamera = !bPrimaryCamera;
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		bViewportFocused = ImGui::IsWindowFocused();
		bViewportHovered = ImGui::IsWindowHovered();
		CApplication::Get().GetImGuiLayer()->SetBlockEvents(!bViewportFocused || !bViewportHovered);

		ImVec2 WindowSize = ImGui::GetContentRegionAvail();
		if (WindowSize.x != ViewportSize.x || WindowSize.y != ViewportSize.y)
		{
			ViewportSize = { WindowSize.x, WindowSize.y };
			FrameBuffer->Resize(static_cast<uint32_t>(ViewportSize.x), static_cast<uint32_t>(ViewportSize.y));
		
			Camera.ResizeBounds(ViewportSize.x, ViewportSize.y);
		}
		ImGui::Image((void*)(uintptr_t)FrameBuffer->GetColourAttachmentRendererID(), { ViewportSize.x, ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void CEditorLayer::OnEvent(CEvent& InEvent)
	{
		Camera.OnEvent(InEvent);
	}
}
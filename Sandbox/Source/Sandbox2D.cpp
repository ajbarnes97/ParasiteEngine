#include "Sandbox2D.h"

#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"
#include "glm/gtc/type_ptr.hpp"


CSandbox2D::CSandbox2D() : CLayer("Sandbox2D")
	, Camera(1.7777f, true)
{
}

void CSandbox2D::OnAttach()
{
	Texture = CTexture2D::Create("Assets/Textures/Checkerboard.png");
	SpriteSheet = CTexture2D::Create("Assets/Textures/TextureSheet.png");

	SubTexture = CSubTexture2D::CreateFromCoords(SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	SFrameBufferSpecification Specification;
	Specification.Width = 1280;
	Specification.Height = 720;

	FrameBuffer = CFrameBuffer::Create(Specification);
}

void CSandbox2D::OnDetach()
{
}

void CSandbox2D::OnUpdate(CTimestep InTimestep)
{
	Camera.OnUpdate(InTimestep);

	FrameBuffer->Bind();

	CRenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1));
	CRenderCommand::Clear();

	CRenderer2D::BeginScene(Camera.GetCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 Colour = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
			CRenderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, Colour);
		}
	}
	CRenderer2D::EndScene();

	CRenderer2D::BeginScene(Camera.GetCamera());
	//CRenderer2D::DrawQuad({ 0.4f, -1.0f, 0.0f }, { 2.0f, 0.4f }, {0.2f, 0.8f, 0.8f, 1.0f});
	//CRenderer2D::DrawQuad({ 0.0f, 1.0f, 0.0f }, { 2.0f, 0.4f }, {0.5f, 0.2f, 0.2f, 1.0f});
	//CRenderer2D::DrawRotatedQuad({ 0.4f, -1.0f, -0.2f }, { 2.0f, 0.4f }, 25.0f, {0.2f, 0.8f, 0.8f, 1.0f});
	//CRenderer2D::DrawRotatedQuad({ -2.0f, -0.0f, 0.0f }, { 1.0f, 1.0f }, -25.0f, Texture, 20.0f, { 0.7f, 0.7, 0.2f, 1.0f });
	CRenderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, SubTexture);
	CRenderer2D::EndScene();

	FrameBuffer->Unbind();
}

void CSandbox2D::OnImGuiRender()
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

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), DockspaceFlags);

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

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Colour:", glm::value_ptr(SqaureColour));
	ImGui::Image((void*)(uintptr_t)FrameBuffer->GetColourAttachmentRendererID(), { 1280.0f, 720.0f }, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void CSandbox2D::OnEvent(CEvent& InEvent)
{
	Camera.OnEvent(InEvent);
}
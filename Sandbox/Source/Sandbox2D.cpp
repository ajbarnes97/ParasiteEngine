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
}

void CSandbox2D::OnDetach()
{
}

void CSandbox2D::OnUpdate(CTimestep InTimestep)
{
	Camera.OnUpdate(InTimestep);

	CRenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1));
	CRenderCommand::Clear();

	CRenderer2D::BeginScene(Camera.GetCamera());

	CRenderer2D::DrawQuad({ 0.4f, -1.0f, 0.0f }, { 2.0f, 0.4f }, {0.2f, 0.8f, 0.8f, 1.0f});
	CRenderer2D::DrawQuad({ 0.0f, 1.0f, 0.0f }, { 2.0f, 0.4f }, {0.5f, 0.2f, 0.2f, 1.0f});
	//CRenderer2D::DrawRotatedQuad({ 0.4f, -1.0f, 0.0f }, { 2.0f, 0.4f }, glm::radians(-25.0f), {0.2f, 0.8f, 0.8f, 1.0f});
	CRenderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, Texture, 10.0f);
	//CRenderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, Texture, 20.0f);

	CRenderer2D::EndScene();
}

void CSandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Colour:", glm::value_ptr(SqaureColour));
	ImGui::End();
}

void CSandbox2D::OnEvent(CEvent& InEvent)
{
	Camera.OnEvent(InEvent);
}
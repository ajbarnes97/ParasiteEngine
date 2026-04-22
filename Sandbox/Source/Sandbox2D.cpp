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
	CRenderer2D::DrawRotatedQuad({ 0.4f, -1.0f, -0.2f }, { 2.0f, 0.4f }, 25.0f, {0.2f, 0.8f, 0.8f, 1.0f});
	CRenderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, Texture, 10.0f, { 1.0f, 0.2, 0.8f, 1.0f });
	CRenderer2D::DrawRotatedQuad({ -2.0f, -0.0f, 0.0f }, { 1.0f, 1.0f }, -25.0f, Texture, 20.0f, { 0.7f, 0.7, 0.2f, 1.0f });

	CRenderer2D::EndScene();

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
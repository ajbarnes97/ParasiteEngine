#pragma once
#include "ParasiteEngine.h"

#include "ParasiteEngine/Core/Timestep.h"
#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Renderer/CameraController.h"

using namespace Parasite;


class CSandbox2D : public CLayer
{
public:
	CSandbox2D();
	virtual ~CSandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(CTimestep InTimestep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(CEvent& InEvent) override;

private:
	COrthographicCameraController Camera;

	TSharedPtr<CVertexArray> VertexArray;
	TSharedPtr<CShader> FlatColourShader;

	TSharedPtr<CTexture2D> Texture;
	TSharedPtr<CTexture2D> SpriteSheet;
	TSharedPtr<CSubTexture2D> SubTexture;
	TSharedPtr<CFrameBuffer> FrameBuffer;

	glm::vec4 SqaureColour = { 0.2f, 0.3f, 0.8f, 1.0f };
};
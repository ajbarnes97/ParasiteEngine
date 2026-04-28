#pragma once
#include "ParasiteEngine.h"

#include "ParasiteEngine/Core/Timestep.h"
#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Renderer/CameraController.h"


namespace Parasite
{
	class CEditorLayer : public CLayer
	{
	public:
		CEditorLayer();
		virtual ~CEditorLayer() = default;

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

		TSharedPtr<CScene> ActiveScene;
		CEntity SqaureEntity;
		CEntity CameraEntity, CameraTwoEntity;

		bool bPrimaryCamera = true;
		bool bViewportFocused = false, bViewportHovered = false;
		glm::vec2 ViewportSize = { 0.0f, 0.0f };

		glm::vec4 SqaureColour = { 0.2f, 0.3f, 0.8f, 1.0f };
	};
}
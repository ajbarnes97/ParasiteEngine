#pragma once
#include "ParasiteEngine.h"

#include "ParasiteEngine/Core/Timestep.h"
#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Renderer/CameraController.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/EditorSettingsPanel.h"
#include "ParasiteEngine/Renderer/EditorCamera.h"


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
		void DrawGizmos();

		bool OnKeyPressed(CPressedKeyEvent& InEvent);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();

		void DrawMainMenuBar();
		void DrawViewport();

		void ApplyEditorTheme();
		ImVec4 ToImVec4(const SColour& InColour);

	private:
		COrthographicCameraController Camera;
		TSharedPtr<CFrameBuffer> FrameBuffer;

		CEditorCamera EditorCamera;

		TSharedPtr<CScene> ActiveScene;
		std::string ActiveSceneFilePath;

		bool bPrimaryCamera = true;
		bool bViewportFocused = false, bViewportHovered = false;
		glm::vec2 ViewportSize = { 0.0f, 0.0f };

		int GizmoType = INDEX_NONE;

		CSceneHierarchyPanel HierarchyPanel;
		CEditorSettingsPanel EditorSettingsPanel;

		SEditorSettings EditorStyle;
		std::string SettingsPath = "EditorSettings.peconfig";
	};
}
#pragma once

#include "ParasiteEngine/Renderer/Camera.h"

#include "ParasiteEngine/Core/Timestep.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"
#include "ParasiteEngine/Events/MouseEvents.h"


namespace Parasite
{
	class COrthographicCameraController
	{
	public:
		COrthographicCameraController(float InAspectRatio, bool bInEnableRotation = false);

		void OnUpdate(CTimestep InTimestep);
		void OnEvent(CEvent& InEvent);

		void ResizeBounds(float InWidth, float InHeight);

		inline const COrthographicCamera& GetCamera() const { return Camera; };
		inline COrthographicCamera& GetCamera() { return Camera; };

	private:
		bool OnMouseScrolledEvent(CMouseScrollEvent& InEvent);
		bool OnWindowResize(CWindowResizeEvent& InEvent);

		inline float GetCameraMoveSpeed(CTimestep InTimestep) const;
		inline float GetCameraRotationSpeed(CTimestep InTimestep) const;

	private:
		float AspectRatio;
		float ZoomLevel = 1.0f;
		bool bRotationEnabled = false;

		float CameraTranslationSpeed = 5.0f;
		float CameraRotationSpeed = 180.0f;

		float CameraRotation = 0.0f;
		glm::vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };

		COrthographicCamera Camera;
	};
}

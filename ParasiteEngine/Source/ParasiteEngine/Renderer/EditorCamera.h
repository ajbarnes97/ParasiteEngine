#pragma once
#include "Camera.h"
#include "ParasiteEngine/Core/Timestep.h"
#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Events/MouseEvents.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"


namespace Parasite
{
	class CEditorCamera
	{
	public:
		CEditorCamera() = default;
		CEditorCamera(float InFOV, float InAspectRatio, float InNearClip, float InFarClip, bool bInOrbiting = false);

		void OnUpdate(CTimestep InTimestep);
		void OnEvent(CEvent& InEvent);

		void SetViewportSize(float InWidth, float InHeight);

		const glm::mat4& GetViewMatrix() const { return ViewMatrix; }
		glm::mat4 GetViewProjection() const { return Projection * ViewMatrix; }
		glm::mat4 GetProjection() const { return Projection; }

		const glm::vec3& GetPosition() const { return Position; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(CMouseScrollEvent& InEvent);

		void MousePan(const glm::vec2& InDelta);
		void MouseRotate(const glm::vec2& InDelta);
		void MouseZoom(float InDelta);

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		glm::quat GetOrientation() const;

	private:
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);

		glm::vec3 Position = { 0.0f, 0.0f, 5.0f };
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };

		glm::vec3 FocalPoint = glm::vec3(0.0f);

		glm::vec2 InitialMousePosition = { 0.0f, 0.0f };

		float Pitch = 0.0f;
		float Yaw = 0.0f;

		float Distance = 10.0f;

		float FOV = 45.0f;
		float AspectRatio = 1.778f;
		float NearClip = 0.1f;
		float FarClip = 1000.0f;

		float ViewportWidth = 1280.0f;
		float ViewportHeight = 720.0f;

		float MoveSpeed = 10.0f;
		float RotationSpeed = 0.003f;
		float ZoomSpeed = 2.0f;

		bool bOrbiting = false;
	};
}
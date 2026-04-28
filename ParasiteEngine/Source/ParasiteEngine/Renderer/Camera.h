#pragma once

#include "glm/glm.hpp"


namespace Parasite
{
	class CCamera
	{
	public:
		CCamera() = default;
		CCamera(const glm::mat4& InProjection)
			: ProjectionMatrix(InProjection) {}
		
		const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return ViewProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return ViewMatrix; }

	protected:
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
	};


	class COrthographicCamera : public CCamera
	{
	public:
		COrthographicCamera(float InLeft, float InRight, float InBottom, float InTop);
		
		void SetProjection(float InLeft, float InRight, float InBottom, float InTop);

		const glm::vec3 GetPosition() const { return Position; }
		void SetPosition(const glm::vec3& InPosition);

		float GetRotation() const { return Rotation; }
		void SetRotation(float InRotation);

	private:
		void RecalculateViewMatrix();

	private:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;

		float NearPlane = -1.0f;
		float FarPlane = 1.0f;
	};


	class CPerspectiveCamera : public CCamera
	{
	public:
		CPerspectiveCamera(float InFov, float InAspectRatio, float InNearClip, float InFarClip);

		void SetProjection(float InFov, float InAspectRatio, float InNearClip, float InFarClip);
		void SetView(const glm::mat4& InView);

	private:
		void RecalculateViewProjection();
	};
}
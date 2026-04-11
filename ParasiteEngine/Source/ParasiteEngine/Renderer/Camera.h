#pragma once

#include "glm/glm.hpp"

namespace Parasite
{
	class COrthographicCamera
	{
	public:
		COrthographicCamera(float InLeft, float InRight, float InBottom, float InTop);

		const glm::vec3 GetPosition() const { return Position; }
		void SetPosition(const glm::vec3& InPosition);

		float GetRotation() const { return Rotation; }
		void SetRotation(float InRotation);

		const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;

		float NearPlane = -1.0f;
		float FarPlane = 1.0f;
	};
}
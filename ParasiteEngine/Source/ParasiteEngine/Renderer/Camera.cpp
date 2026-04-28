#include "pepch.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Parasite
{
	COrthographicCamera::COrthographicCamera(float InLeft, float InRight, float InBottom, float InTop)
	{
		SetProjection(InLeft, InRight, InBottom, InTop);
		RecalculateViewMatrix();
	}

	void COrthographicCamera::SetProjection(float InLeft, float InRight, float InBottom, float InTop)
	{
		ProjectionMatrix = glm::ortho(InLeft, InRight, InBottom, InTop);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

	void COrthographicCamera::SetPosition(const glm::vec3& InPosition)
	{
		Position = InPosition;
		RecalculateViewMatrix();
	}

	void COrthographicCamera::SetRotation(float InRotation)
	{
		Rotation = InRotation;
		RecalculateViewMatrix();
	}

	void COrthographicCamera::RecalculateViewMatrix()
	{
		const glm::vec3 AxisZ = glm::vec3(0, 0, 1);
		
		glm::mat4 Transform = 
			glm::translate(glm::mat4(1.0f), Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), AxisZ);

		ViewMatrix = glm::inverse(Transform);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}


	CPerspectiveCamera::CPerspectiveCamera(float InFov, float InAspectRatio, float InNearClip, float InFarClip)
	{
		ProjectionMatrix = glm::perspective(glm::radians(InFov), InAspectRatio, InNearClip, InFarClip);

		ViewMatrix = glm::mat4(1.0f);
		RecalculateViewProjection();
	}

	void CPerspectiveCamera::SetProjection(float InFov, float InAspectRatio, float InNearClip, float InFarClip)
	{
		ProjectionMatrix = glm::perspective(glm::radians(InFov), InAspectRatio, InNearClip, InFarClip);
		RecalculateViewProjection();
	}

	void CPerspectiveCamera::SetView(const glm::mat4& InView)
	{
		ViewMatrix = InView;
		RecalculateViewProjection();
	}

	void CPerspectiveCamera::RecalculateViewProjection()
	{
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
}
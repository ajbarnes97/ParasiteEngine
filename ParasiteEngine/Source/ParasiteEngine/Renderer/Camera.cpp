#include "pepch.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Parasite
{

	void CCamera::SetOrthographic(float InSize, float InNearClip, float InFarClip)
	{
		ProjectionType = EProjectionType::Orthographic;
		OrthographicSize = InSize;
		OrthographicNearClip = InNearClip;
		OrthographicFarClip = InFarClip;
		RecalculateProjection();
	}

	void CCamera::SetPerspective(float InFieldOfView, float InNearClip, float InFarClip)
	{
		ProjectionType = EProjectionType::Perspective;
		PerspectiveFOV = InFieldOfView;
		PerspectiveNearClip = InNearClip;
		PerspectiveFarClip = InFarClip;
		RecalculateProjection();
	}

	void CCamera::SetViewportSize(float InWidth, float InHeight)
	{
		AspectRatio = InWidth / InHeight;
		RecalculateProjection();
	}

	void CCamera::SetProjectionType(EProjectionType InType)
	{
		ProjectionType = InType;
		RecalculateProjection();
	}

	void CCamera::SetPerspectiveFOV(float InFieldOfView)
	{
		PerspectiveFOV = InFieldOfView; 
		RecalculateProjection();
	}

	void CCamera::SetPerspectiveNearClip(float InNearClip)
	{
		PerspectiveNearClip = InNearClip;
		RecalculateProjection();
	}

	void CCamera::SetPerspectiveFarClip(float InFarClip)
	{
		PerspectiveFarClip = InFarClip;
		RecalculateProjection();
	}

	void CCamera::SetOrthographicSize(float InSize)
	{
		OrthographicSize = InSize; 
		RecalculateProjection();
	}

	void CCamera::SetOrthographicNearClip(float InNearClip)
	{
		OrthographicNearClip = InNearClip;
		RecalculateProjection();
	}

	void CCamera::SetOrthographicFarClip(float InFarClip)
	{
		OrthographicFarClip = InFarClip;
		RecalculateProjection();
	}

	void CCamera::RecalculateProjection()
	{
		if (ProjectionType == EProjectionType::Perspective)
		{
			ProjectionMatrix = glm::perspective(glm::radians(PerspectiveFOV), AspectRatio,
				PerspectiveNearClip, PerspectiveFarClip);
		}
		else
		{
			const float left = -OrthographicSize * AspectRatio * 0.5f;
			const float right = OrthographicSize * AspectRatio * 0.5f;
			const float bottom = -OrthographicSize * 0.5f;
			const float top = OrthographicSize * 0.5f;

			ProjectionMatrix = glm::ortho(left, right, bottom, top, OrthographicNearClip, OrthographicFarClip);
		}
	}
}
#pragma once

#include "glm/glm.hpp"


namespace Parasite
{
	class CCamera
	{
	public:
		enum class EProjectionType
		{
			Perspective = 0,
			Orthographic = 1,
		};

		void SetOrthographic(float InSize, float InNearClip, float InFarClip);
		void SetPerspective(float InFieldOfView, float InNearClip, float InFarClip);

		void SetViewportSize(float InWidth, float InHeight);

		const inline EProjectionType GetProjectionType() const { return ProjectionType; }
		void SetProjectionType(EProjectionType InType);

		const inline float GetPerspectiveFOV() const { return PerspectiveFOV; }
		void SetPerspectiveFOV(float InFieldOfView);

		const inline float GetPerspectiveNearClip() const { return PerspectiveNearClip; }
		void SetPerspectiveNearClip(float InNearClip);
		const inline float GetPerspectiveFarClip() const { return PerspectiveFarClip; }
		void SetPerspectiveFarClip(float InFarClip);

		const inline float GetOrthographicSize() const { return OrthographicSize; }
		void SetOrthographicSize(float InSize);

		const inline float GetOrthographicNearClip() const { return OrthographicNearClip; }
		void SetOrthographicNearClip(float InNearClip);
		const inline float GetOrthographicFarClip() const { return OrthographicFarClip; }
		void SetOrthographicFarClip(float InFarClip);

		const inline glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

	private:
		void RecalculateProjection();

	private:
		EProjectionType ProjectionType = EProjectionType::Perspective;

		float PerspectiveFOV = 45.0f;
		float PerspectiveNearClip = 0.1f;
		float PerspectiveFarClip = 1000.0f;

		float OrthographicSize = 10.0f;
		float OrthographicNearClip = -1.0f;
		float OrthographicFarClip = 1.0f;

		float AspectRatio = 1.778f;

		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	};
}
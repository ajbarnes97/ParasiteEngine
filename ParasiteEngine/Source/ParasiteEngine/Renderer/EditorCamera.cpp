#include "pepch.h"
#include "EditorCamera.h"

#include "ParasiteEngine/Core/Input.h"
#include "ParasiteEngine/Core/KeyCodes.h"
#include "ParasiteEngine/Core/Core.h"


namespace Parasite
{
	CEditorCamera::CEditorCamera(float InFOV, float InAspectRatio, float InNearClip, float InFarClip, bool bInOrbiting)
		: FOV(InFOV), AspectRatio(InAspectRatio), NearClip(InNearClip), FarClip(InFarClip), bOrbiting(bInOrbiting),
			Projection(glm::perspective(glm::radians(FOV), AspectRatio, NearClip, FarClip))
	{
		UpdateProjection();
		UpdateView();
	}

	void CEditorCamera::SetViewportSize(float InWidth, float InHeight)
	{
		ViewportWidth = InWidth;
		ViewportHeight = InHeight;

		UpdateProjection();
	}

	void CEditorCamera::UpdateProjection()
	{
		if (ViewportWidth == 0 || ViewportHeight == 0)
		{
			return;
		}

		AspectRatio = ViewportWidth / ViewportHeight;
		Projection = glm::perspective(glm::radians(FOV), AspectRatio, NearClip, FarClip);
	}

	void CEditorCamera::OnUpdate(CTimestep InTimestep)
	{
		float DeltaTime = static_cast<float>(InTimestep);

		glm::vec2 MousePosition = { CInput::GetMouseX(), CInput::GetMouseY() };
		glm::vec2 Delta = (MousePosition - InitialMousePosition) * RotationSpeed;
		InitialMousePosition = MousePosition;

		CInput::IsKeyPressed(PE_KEY_LEFT_ALT);
		if (bOrbiting)
		{
			if (CInput::IsMousePressed(0))
			{
				MouseRotate(Delta);
			}
			else if (CInput::IsMousePressed(2))
			{
				MousePan(Delta);
			}

			Position = FocalPoint - GetForwardDirection() * Distance;
		}
		else if(CInput::IsMousePressed(1))
		{
			MouseRotate(Delta);
			glm::vec3 Direction(0.0f);

			if (CInput::IsKeyPressed(PE_KEY_W))
			{
				Direction += GetForwardDirection();
			}
			if (CInput::IsKeyPressed(PE_KEY_S))
			{
				Direction -= GetForwardDirection();
			}
			if (CInput::IsKeyPressed(PE_KEY_D))
			{
				Direction += GetRightDirection();
			}
			if (CInput::IsKeyPressed(PE_KEY_A))
			{
				Direction -= GetRightDirection();
			}

			if (glm::length(Direction) > 0.0f)
			{
				Direction = glm::normalize(Direction);
			}

			float Speed = MoveSpeed;
			if (CInput::IsKeyPressed(PE_KEY_LEFT_SHIFT))
			{
				Speed *= 0.5f;
			}
			glm::vec3 TargetVelocity = Direction * Speed;

			// Smoothly accelerate/decelerate
			Velocity = glm::mix(Velocity, TargetVelocity, 10.0f * DeltaTime);
			Position += Velocity * DeltaTime;
		}
		else
		{
			// Decelerate to stop
			Velocity = glm::mix(Velocity, glm::vec3(0.0f), 8.0f * DeltaTime);
		}

		UpdateView();
	}

	void CEditorCamera::UpdateView()
	{
		glm::quat Orientation = GetOrientation();
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(Orientation);
		ViewMatrix = glm::inverse(Transform);
	}

	void CEditorCamera::OnEvent(CEvent& InEvent)
	{
		CEventDispatcher Dispatcher(InEvent);
		Dispatcher.Dispatch<CMouseScrollEvent>(PE_BIND_EVENT_FUNC(CEditorCamera::OnMouseScroll));
	}

	bool CEditorCamera::OnMouseScroll(CMouseScrollEvent& InEvent)
	{
		const float Delta = InEvent.GetYOffset();
		if (bOrbiting)
		{
			Distance -= Delta * ZoomSpeed;
			Distance = std::max(Distance, 1.0f);
		}
		else
		{
			MoveSpeed += Delta * 2.0f;
			MoveSpeed = std::max(MoveSpeed, 1.0f);
		}

		return false;
	}

	void CEditorCamera::MousePan(const glm::vec2& InDelta)
	{
		float PanSpeed = Distance * 0.001f;

		FocalPoint -= GetRightDirection() * InDelta.x * PanSpeed;
		FocalPoint += GetUpDirection() * InDelta.y * PanSpeed;
	}

	void CEditorCamera::MouseRotate(const glm::vec2& InDelta)
	{
		const float YawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		Yaw += YawSign * InDelta.x;
		Pitch += InDelta.y;

		Pitch = glm::clamp(Pitch, -glm::radians(89.0f), glm::radians(89.0f));
	}

	void CEditorCamera::MouseZoom(float InDelta)
	{
		Distance -= InDelta * ZoomSpeed;
		if (Distance < 1.0f)
		{
			Distance = 1.0f;
		}
	}

	glm::vec3 CEditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 CEditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 CEditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat CEditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-Pitch, -Yaw, 0.0f));
	}
}
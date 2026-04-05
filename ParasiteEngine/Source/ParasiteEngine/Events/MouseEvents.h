#pragma once

#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Core.h"


namespace Parasite
{
	class PARASITE_API CMouseMoveEvent : public CEvent
	{
	public:
		CMouseMoveEvent(float InX, float InY)
			: MouseX(InX),
				MouseY(InY)
		{
		}

		inline float GetX() const { return MouseX; }
		inline float GetY() const { return MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream SS;
			SS << "Mouse Move Event: " << GetX() << ", " << GetY();
			return SS.str();
		}

		EVENT_CLASS_TYPE(MouseMove);
		EVENT_CLASS_CATEGORY(CategoryInput | CategoryMouse);
	
	private:
		float MouseX, MouseY;
	};


	class PARASITE_API CMouseScrollEvent : public CEvent
	{
	public:
		CMouseScrollEvent(float InXOffset, float InYOffset)
			: XOffset(InXOffset),
				YOffset(InYOffset)
		{
		}

		inline float GetXOffset() const { return XOffset; }
		inline float GetYOffset() const { return YOffset; }

		virtual std::string ToString() const override
		{
			std::stringstream SS;
			SS << "Mouse Scroll Event: " << GetXOffset() << ", " << GetYOffset();
			return SS.str();
		}

		EVENT_CLASS_TYPE(MouseScroll);
		EVENT_CLASS_CATEGORY(CategoryInput | CategoryMouse);

	private:
		float XOffset, YOffset;
	};


	class PARASITE_API CMouseButtonEvent : public CEvent
	{
	public:
		inline int GetMouseButton() const { return Button; }

		EVENT_CLASS_CATEGORY(CategoryInput | CategoryMouse)

	protected:
		CMouseButtonEvent(int InButton)
			: Button(InButton)
		{
		}

	protected:
		int Button;
	};


	class PARASITE_API CMousePressedEvent : public CMouseButtonEvent
	{
	public:
		CMousePressedEvent(int InButton)
			: CMouseButtonEvent(InButton)
		{
		}

		virtual std::string ToString() const override
		{
			return "Mouse Pressed Event";
		}

		EVENT_CLASS_TYPE(MousePressed);
	};


	class PARASITE_API CMouseReleasedEvent : public CMouseButtonEvent
	{
	public:
		CMouseReleasedEvent(int InButton)
			: CMouseButtonEvent(InButton)
		{
		}

		virtual std::string ToString() const override
		{
			return "Mouse Released Event";
		}

		EVENT_CLASS_TYPE(MouseReleased);
	};
}
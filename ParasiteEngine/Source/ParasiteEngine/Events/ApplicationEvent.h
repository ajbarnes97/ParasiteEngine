#pragma once

#include "ParasiteEngine/Events/Event.h"


namespace Parasite
{
	class PARASITE_API CWindowResizeEvent : public CEvent
	{
	public:
		CWindowResizeEvent(unsigned int InWindowWidth, unsigned int InWindowHeight)
			: WindowWidth(InWindowWidth),
			  WindowHeight(InWindowHeight)
		{
		}

		inline unsigned int GetWidth() const { return WindowWidth; }
		inline unsigned int GetHeight() const { return WindowHeight; }

		virtual std::string ToString() const override
		{
			std::stringstream SS;
			SS << "Window Resize Event: " << GetWidth() << ", " << GetHeight();
			return SS.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(CategoryApplication);
	private:
		unsigned int WindowWidth, WindowHeight;
	};


	class PARASITE_API CWindowCloseEvent : public CEvent
	{
	public:
		CWindowCloseEvent() = default;

		virtual std::string ToString() const override
		{
			return "Window Close Event";
		}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(CategoryApplication);
	};
}
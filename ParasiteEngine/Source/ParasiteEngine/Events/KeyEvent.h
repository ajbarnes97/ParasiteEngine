#pragma once

#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Core.h"


namespace Parasite
{
	class PARASITE_API CKeyEvent : public CEvent
	{
	public:
		inline int GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(CategoryInput | CategoryKeyboard)

	protected:
		CKeyEvent(int InKeyCode)
			: KeyCode(InKeyCode)
		{
		}

	protected:
		int KeyCode;
	};


	class PARASITE_API CPressedKeyEvent : public CKeyEvent
	{
	public:
		CPressedKeyEvent(int InKeyCode, int InRepeatCount)
			: CKeyEvent(InKeyCode),
				RepeatCount(InRepeatCount)
		{
		}

		inline int GetRepeatCount() const { return RepeatCount; }

		virtual std::string ToString() const override
		{
			std::stringstream SS;
			SS << "Pressed Key Event| Repeat count : " << GetRepeatCount();
			return SS.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	private:
		int RepeatCount = 0;
	};


	class PARASITE_API CReleasedKeyEvent : public CKeyEvent
	{
	public:
		CReleasedKeyEvent(int InKeyCode)
			: CKeyEvent(InKeyCode)
		{
		}

		virtual std::string ToString() const override
		{
			return "Released Key Event";
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};
}
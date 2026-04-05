#pragma once

#include "ParasiteEngine/Core.h"


namespace Parasite
{
	enum class EEventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFoucs, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MousePressed, MouseReleased, MouseMove, MouseScroll,
	};

	enum EEventCategory
	{
		None = 0,
		CategoryApplication		= BIT(0),
		CategoryInput			= BIT(1),
		CategoryKeyboard		= BIT(2),
		CategoryMouse			= BIT(3),
		CategoryMouseButton		= BIT(4),
	};


#define EVENT_CLASS_TYPE(InType)	static EEventType GetStaticType() { return EEventType::##InType; }\
									virtual EEventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const { return #InType; }

#define EVENT_CLASS_CATEGORY(InCategory) virtual int GetCategoryFlags() const { return InCategory; }


	class PARASITE_API CEvent
	{
		friend class CEventDispatcher;

	public:
		virtual EEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EEventCategory InCategory) const
		{
			return GetCategoryFlags() & InCategory;
		}

		inline const bool IsHandled() const { return bHandled; }

	protected:
		bool bHandled = false;
	};


	class PARASITE_API CEventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;

	public:
		CEventDispatcher(CEvent& InEvent)
			: Event(InEvent)
		{
		}

		template<typename T>
		bool Dispatch(EventFunc<T> InFunc)
		{
			if (Event.GetEventType() == T::GetStaticType())
			{
				Event.bHandled = InFunc(*(T*)&Event);
				return true;
			}
			return false;
		}

	private:
		CEvent& Event;
	};
}
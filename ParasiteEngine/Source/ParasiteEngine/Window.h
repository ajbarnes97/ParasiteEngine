#pragma once

#include "pepch.h"

#include "Core.h"
#include "Events/Event.h"


namespace Parasite
{
	struct SWindowInfo
	{
		SWindowInfo(const std::string& InTitle = "Parasite Engine",
					unsigned int InWidth = 1280,
					unsigned int InHeight = 720)
			: Title(InTitle),
				Width(InWidth),
				Height(InHeight)
		{

		}

		std::string Title;
		unsigned int Width;
		unsigned int Height;
	};

	
	/**
	 * Base interface for a desktop system based window
	 */
	class PARASITE_API CWindow
	{
	public:
		using EventCallbackFunc = std::function<void(CEvent&)>;

		virtual ~CWindow() {};

		virtual void Update() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& InCallbackFunc) = 0;
		virtual void SetVSyncEnabled(bool bInEnabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		static CWindow* Create(const SWindowInfo& InInfo = SWindowInfo());
	};
}
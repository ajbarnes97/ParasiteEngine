#pragma once

#include "pepch.h"

#include "Core.h"
#include "ParasiteEngine/Events/Event.h"


namespace Parasite
{
	struct SWindowInfo
	{
		SWindowInfo(const std::string& InTitle = "Parasite Engine",
					uint32_t InWidth = 1280,
					uint32_t InHeight = 720)
			: Title(InTitle),
				Width(InWidth),
				Height(InHeight)
		{
		}

		std::string Title;
		uint32_t Width;
		uint32_t Height;
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

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& InCallbackFunc) = 0;
		virtual void SetVSyncEnabled(bool bInEnabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static CWindow* Create(const SWindowInfo& InInfo = SWindowInfo());
	};
}
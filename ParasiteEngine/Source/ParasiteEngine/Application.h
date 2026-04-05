#pragma once

#include "Core.h"
#include "Window.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"

namespace Parasite
{
	class PARASITE_API CApplication
	{
	public:
		CApplication();
		virtual ~CApplication();

		void Run();
		void OnEvent(CEvent& InEvent);

	private:
		bool OnWindowClose(CWindowCloseEvent& InWindowCloseEvent);

	private:
		std::unique_ptr<CWindow> Window;
		bool bRunning = true;
	};

	// To be defined in client
	CApplication* CreateApplication();
}
#pragma once

#include "Core.h"
#include "Window.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"
#include "ParasiteEngine/LayerStack.h"


namespace Parasite
{
	class PARASITE_API CApplication
	{
	public:
		CApplication();
		virtual ~CApplication();

		void Run();
		void OnEvent(CEvent& InEvent);

		void PushLayer(CLayer* InLayer);
		void PushOverlay(CLayer* InOverlay);

	private:
		bool OnWindowClose(CWindowCloseEvent& InWindowCloseEvent);

	private:
		CLayerStack LayerStack;

		std::unique_ptr<CWindow> Window;
		bool bRunning = true;
	};

	// To be defined in client
	CApplication* CreateApplication();
}
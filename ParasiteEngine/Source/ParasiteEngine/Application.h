#pragma once

#include "Core.h"
#include "Window.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"
#include "ParasiteEngine/LayerStack.h"
#include "ParasiteEngine/ImGui/ImGuiLayer.h"


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

		inline static CApplication& Get() { return *Instance; }
		inline CWindow& GetWindow() { return *Window; }

	private:
		bool OnWindowClose(CWindowCloseEvent& InWindowCloseEvent);

	private:
		CLayerStack LayerStack;

		TWeakPtr<CWindow> Window;
		CImGuiLayer* ImGuiLayer;
		bool bRunning = true;
		
		float LastFrameTime = 0.0f;

		static CApplication* Instance;
	};

	// To be defined in client
	CApplication* CreateApplication();
}
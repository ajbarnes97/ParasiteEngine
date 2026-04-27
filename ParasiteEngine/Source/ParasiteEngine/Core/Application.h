#pragma once

#include "Core.h"
#include "Window.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"
#include "ParasiteEngine/Core/LayerStack.h"
#include "ParasiteEngine/ImGui/ImGuiLayer.h"


namespace Parasite
{
	class PARASITE_API CApplication
	{
	public:
		CApplication(const std::string& InWindowName = "Parasite Engine");
		virtual ~CApplication();

		void Run();
		void Close();

		void OnEvent(CEvent& InEvent);

		void PushLayer(CLayer* InLayer);
		void PushOverlay(CLayer* InOverlay);

		inline static CApplication& Get() { return *Instance; }
		inline CWindow& GetWindow() { return *Window; }

		inline CImGuiLayer* GetImGuiLayer() { return ImGuiLayer; };

	private:
		bool OnWindowClose(CWindowCloseEvent& InWindowCloseEvent);
		bool OnWindowResize(CWindowResizeEvent& InWindowResizeEvent);

	private:
		CLayerStack LayerStack;

		TWeakPtr<CWindow> Window;
		CImGuiLayer* ImGuiLayer;
		bool bRunning = true;
		bool bMinimized = false;
		
		float LastFrameTime = 0.0f;

		static CApplication* Instance;
	};

	// To be defined in client
	CApplication* CreateApplication();
}
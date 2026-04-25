#include "pepch.h"
#include "Application.h"

#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Core/Log.h"
#include "ParasiteEngine/Core/KeyCodes.h"
#include "ParasiteEngine/Core/Timestep.h"
#include "ParasiteEngine/Renderer/Renderer.h"

#include "GLFW/glfw3.h"


namespace Parasite
{
	CApplication* CApplication::Instance = nullptr;

	CApplication::CApplication(const std::string& InWindowName)
	{
		PE_CORE_ASSERT(!Instance, "An Application already exists.");
		Instance = this;

		Window = TWeakPtr<CWindow>(CWindow::Create(SWindowInfo(InWindowName)));
		Window->SetEventCallback(std::bind(&CApplication::OnEvent, this, std::placeholders::_1));

		CRenderer::Init();

		ImGuiLayer = new CImGuiLayer();
		PushOverlay(ImGuiLayer);
	}

	CApplication::~CApplication()
	{
	}

	void CApplication::Run()
	{
		while (bRunning)
		{
			const float Time = static_cast<float>(glfwGetTime()); // todo: platfrom::GetTime
			CTimestep Timestep = Time - LastFrameTime;
			LastFrameTime = Time;

			if (!bMinimized)
			{
				for (CLayer* Layer : LayerStack)
				{
					Layer->OnUpdate(Timestep);
				}
			}

			ImGuiLayer->Begin();
			for (CLayer* Layer : LayerStack)
			{
				Layer->OnImGuiRender();
			}
			ImGuiLayer->End();

			Window->Update();
		}
	}


	void CApplication::Close()
	{
		bRunning = false;
	}

	void CApplication::OnEvent(CEvent& InEvent)
	{
		CEventDispatcher EventDispatcher(InEvent);
		EventDispatcher.Dispatch<CWindowCloseEvent>(PE_BIND_EVENT_FUNC(CApplication::OnWindowClose));
		EventDispatcher.Dispatch<CWindowResizeEvent>(PE_BIND_EVENT_FUNC(CApplication::OnWindowResize));

		for (auto It = LayerStack.end(); It != LayerStack.begin(); )
		{
			if (CLayer* Layer = *--It)
			{
				Layer->OnEvent(InEvent);
				if (InEvent.IsHandled())
				{
					break;
				}
			}
		}
	}

	void CApplication::PushLayer(CLayer* InLayer)
	{
		LayerStack.PushLayer(InLayer);
	}

	void CApplication::PushOverlay(CLayer* InOverlay)
	{
		LayerStack.PushOverlay(InOverlay);
	}

	bool CApplication::OnWindowClose(CWindowCloseEvent& InWindowCloseEvent)
	{
		bRunning = false;
		return true;
	}

	bool CApplication::OnWindowResize(CWindowResizeEvent& InWindowResizeEvent)
	{
		if (InWindowResizeEvent.GetWidth() <= 0 || InWindowResizeEvent.GetHeight() <= 0)
		{
			bMinimized = true;
			return false;
		}

		bMinimized = false;
		CRenderer::OnWindowResize(InWindowResizeEvent.GetWidth(), InWindowResizeEvent.GetHeight());

		return false;
	}
}
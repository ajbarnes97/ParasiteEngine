#include "pepch.h"
#include "Application.h"

#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Log.h"
#include "ParasiteEngine/KeyCodes.h"
#include "ParasiteEngine/Core/Timestep.h"

#include "GLFW/glfw3.h"


namespace Parasite
{
	CApplication* CApplication::Instance = nullptr;

	CApplication::CApplication()
	{
		PE_CORE_ASSERT(!Instance, "An Application already exists.");
		Instance = this;

		Window = TWeakPtr<CWindow>(CWindow::Create());
		Window->SetEventCallback(std::bind(&CApplication::OnEvent, this, std::placeholders::_1));

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

			for (CLayer* Layer : LayerStack)
			{
				Layer->OnUpdate(Timestep);
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

	void CApplication::OnEvent(CEvent& InEvent)
	{
		CEventDispatcher EventDispatcher(InEvent);
		EventDispatcher.Dispatch<CWindowCloseEvent>(std::bind(&CApplication::OnWindowClose, this, std::placeholders::_1));

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
}
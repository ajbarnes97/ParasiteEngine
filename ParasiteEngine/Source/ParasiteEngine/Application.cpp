#include "pepch.h"
#include "Application.h"

#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Log.h"
#include "ParasiteEngine/KeyCodes.h"

#include "Glad/glad.h"

#include "Input.h"


namespace Parasite
{
	CApplication* CApplication::Instance = nullptr;

	CApplication::CApplication()
	{
		PE_CORE_ASSERT(!Instance, "An Application already exists.");
		Instance = this;

		Window = std::unique_ptr<CWindow>(CWindow::Create());
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (CLayer* Layer : LayerStack)
			{
				Layer->OnUpdate();
			}

			ImGuiLayer->Begin();
			for (CLayer* Layer : LayerStack)
			{
				Layer->OnImGuiRender();
			}
			ImGuiLayer->End();

			if (Parasite::CInput::IsKeyPressed(PE_KEY_A))
			{
				PE_CORE_LOG("Pressed A!");
			}

			Window->Update();
		}
	}

	void CApplication::OnEvent(CEvent& InEvent)
	{
		CEventDispatcher EventDispatcher(InEvent);
		EventDispatcher.Dispatch<CWindowCloseEvent>(std::bind(&CApplication::OnWindowClose, this, std::placeholders::_1));
	
		for (auto It = LayerStack.end(); It != LayerStack.begin(); )
		{
			if (CLayer * Layer = *--It)
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
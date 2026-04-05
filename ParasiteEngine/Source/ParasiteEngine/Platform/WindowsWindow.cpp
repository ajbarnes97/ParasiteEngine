#include "pepch.h"

#include "WindowsWindow.h"

#include "ParasiteEngine/Log.h"
#include "ParasiteEngine/Events/Event.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"
#include "ParasiteEngine/Events/KeyEvent.h"
#include "ParasiteEngine/Events/MouseEvents.h"


namespace Parasite
{
	static bool bIsGLFWInitialized = false;

	static void HandleGLFWError(int InErrorCode, const char* InDebugMsg)
	{
		PE_CORE_ERROR("GLFW Error: {0}: {1}", InErrorCode, InDebugMsg);
	}

	CWindow* CWindow::Create(const SWindowInfo& InInfo)
	{
		return new CWindowsWindow(InInfo);
	}

	CWindowsWindow::CWindowsWindow(const SWindowInfo& InInfo)
	{
		Init(InInfo);
	}

	CWindowsWindow::~CWindowsWindow()
	{
		Shutdown();
	}

	void CWindowsWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
	}
	
	void CWindowsWindow::SetVSyncEnabled(bool bInEnabled)
	{
		glfwSwapInterval(static_cast<int>(bInEnabled));
		WindowData.bVSyncEnabled = bInEnabled;
	}

	void CWindowsWindow::Init(const SWindowInfo& InInfo)
	{
		WindowData = InInfo;

		PE_CORE_LOG("Creating window {0} ({1}, {2})", WindowData.Title, WindowData.Width, WindowData.Height);
	
		if (!bIsGLFWInitialized)
		{
			const int Success = glfwInit();

			PE_CORE_ASSERT(Success, "Failed to init GLFW window");
			glfwSetErrorCallback(HandleGLFWError);
			bIsGLFWInitialized = true;
		}
	
		Window = glfwCreateWindow(static_cast<int>(WindowData.Width), static_cast<int>(WindowData.Height), WindowData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(Window);
		glfwSetWindowUserPointer(Window, &WindowData);
		SetVSyncEnabled(true);

		// Setup GLFW callbacks
		glfwSetWindowSizeCallback(Window,
			[](GLFWwindow* InWindow, int InWidth, int InHeight)
			{
				SWindowData& Data = *static_cast<SWindowData*>(glfwGetWindowUserPointer(InWindow));
				Data.Width = InWidth;
				Data.Height = InHeight;
				
				CWindowResizeEvent NewEvent(InWidth, InHeight);
				Data.EventCallback(NewEvent);
			}
		);

		glfwSetWindowCloseCallback(Window,
			[](GLFWwindow* InWindow)
			{
				SWindowData& Data = *static_cast<SWindowData*>(glfwGetWindowUserPointer(InWindow));

				CWindowCloseEvent NewEvent;
				Data.EventCallback(NewEvent);
			}
		);
	
		glfwSetKeyCallback(Window, 
			[](GLFWwindow* InWindow, int InKey, int InScancode, int InAction, int InMods)
			{
				SWindowData& Data = *static_cast<SWindowData*>(glfwGetWindowUserPointer(InWindow));

				switch (InAction)
				{
					case GLFW_PRESS:
					{
						CPressedKeyEvent NewEvent(InKey, 0);
						Data.EventCallback(NewEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						CReleasedKeyEvent NewEvent(InKey);
						Data.EventCallback(NewEvent);
						break;
					}
					case GLFW_REPEAT:
					{
						CPressedKeyEvent NewEvent(InKey, 1);
						Data.EventCallback(NewEvent);
						break;
					}
				}
			}
		);

		glfwSetMouseButtonCallback(Window, 
			[](GLFWwindow* InWindow, int InButton, int InAction, int InMods)
			{
				SWindowData& Data = *static_cast<SWindowData*>(glfwGetWindowUserPointer(InWindow));

				switch (InAction)
				{
				case GLFW_PRESS:
				{
					CMousePressedEvent NewEvent(InButton);
					Data.EventCallback(NewEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					CMouseReleasedEvent NewEvent(InButton);
					Data.EventCallback(NewEvent);
					break;
				}
				}
			}
		);

		glfwSetScrollCallback(Window,
			[](GLFWwindow* InWindow, double InXOffset, double InYOffset)
			{
				SWindowData& Data = *static_cast<SWindowData*>(glfwGetWindowUserPointer(InWindow));

				CMouseScrollEvent NewEvent(static_cast<float>(InXOffset), static_cast<float>(InYOffset));
				Data.EventCallback(NewEvent);
			}
		);

		glfwSetCursorPosCallback(Window,
			[](GLFWwindow* InWindow, double InXPos, double InYPos)
			{
				SWindowData& Data = *static_cast<SWindowData*>(glfwGetWindowUserPointer(InWindow));

				CMouseMoveEvent NewEvent(static_cast<float>(InXPos), static_cast<float>(InYPos));
				Data.EventCallback(NewEvent);
			}
		);
	}

	void CWindowsWindow::Shutdown()
	{
		glfwDestroyWindow(Window);
	}
}
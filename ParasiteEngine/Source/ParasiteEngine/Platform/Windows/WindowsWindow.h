#pragma once

#include "ParasiteEngine/Core/Window.h"
#include "ParasiteEngine/Core/Core.h"

#include "GLFW/glfw3.h"

#include "ParasiteEngine/Renderer/GraphicContext.h"


namespace Parasite
{
	class PARASITE_API CWindowsWindow : public CWindow
	{
	public:
		CWindowsWindow(const SWindowInfo& InInfo);
		virtual ~CWindowsWindow() override;

		virtual void Update() override;

		virtual uint32_t GetWidth() const override { return WindowData.Width; };
		virtual uint32_t GetHeight() const override { return WindowData.Height; };

		virtual void SetEventCallback(const EventCallbackFunc& InCallbackFunc) override { WindowData.EventCallback = InCallbackFunc; };
		virtual void SetVSyncEnabled(bool bInEnabled) override;
		virtual bool IsVSyncEnabled() const override { return WindowData.bVSyncEnabled; };

		inline virtual void* GetNativeWindow() const override { return Window; }

	private:
		void Init(const SWindowInfo& InInfo);
		void Shutdown();

	private:
		GLFWwindow* Window;
		CGraphicContext* Context;

		struct SWindowData
		{
			SWindowData() = default;
			SWindowData(const SWindowInfo& InInfo)
			{
				Width = InInfo.Width;
				Height = InInfo.Height;
				Title = InInfo.Title;
			}

			uint32_t Width;
			uint32_t Height;
			std::string Title;

			bool bVSyncEnabled = true;

			EventCallbackFunc EventCallback;
		};

		SWindowData WindowData;
	};
}
#include "pepch.h"
#include "ImGuiLayer.h"

#include "ParasiteEngine/Core/Application.h"
#include "ParasiteEngine/Events/Event.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"


namespace Parasite
{

	CImGuiLayer::CImGuiLayer()
		: CLayer("ImGuiLayer")
	{
	}

	CImGuiLayer::~CImGuiLayer()
	{

	}

	void CImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO(); (void)IO;
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& Style = ImGui::GetStyle();
		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			Style.WindowRounding = 0.0f;
			Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		CApplication& Application = CApplication::Get();
		GLFWwindow* Window = static_cast<GLFWwindow*>(Application.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(Window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void CImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void CImGuiLayer::OnEvent(CEvent& InEvent)
	{
		if (bBlockEvents)
		{
			ImGuiIO& IO = ImGui::GetIO();
			InEvent.bHandled |= InEvent.IsInCategory(EEventCategory::CategoryMouse) & IO.WantCaptureMouse;
			InEvent.bHandled |= InEvent.IsInCategory(EEventCategory::CategoryKeyboard) & IO.WantCaptureKeyboard;
		}
	}

	void CImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void CImGuiLayer::End()
	{
		ImGuiIO& IO = ImGui::GetIO();
		CApplication& Application = CApplication::Get();
		IO.DisplaySize = ImVec2(static_cast<float>(Application.GetWindow().GetWidth()), static_cast<float>(Application.GetWindow().GetHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* Context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(Context);
		}
	}
}
#include "pepch.h"
#include "ImGuiLayer.h"

#include "ParasiteEngine/Core/Application.h"
#include "ParasiteEngine/Events/Event.h"

#include "imgui.h"
#include "ImGuizmo.h"
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

		IO.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Black.ttf", 15.0f);
		IO.FontDefault = IO.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Regular.ttf", 15.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& Style = ImGui::GetStyle();
		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			Style.WindowRounding = 0.0f;
			Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ApplyEditorTheme();

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
		ImGuizmo::BeginFrame();
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

	ImVec4 CImGuiLayer::ToImVec4(const SColour& InColour)
	{
		return ImVec4(InColour.R, InColour.G, InColour.B, InColour.A);
	}

	void CImGuiLayer::ApplyEditorTheme()
	{
		auto& StyleColours = ImGui::GetStyle().Colors;
		StyleColours[ImGuiCol_WindowBg] = ToImVec4(EditorStyle.Background);
		StyleColours[ImGuiCol_Border] = ToImVec4(EditorStyle.Border);
		
		// Headers
		StyleColours[ImGuiCol_Header] = ToImVec4(EditorStyle.Header);
		StyleColours[ImGuiCol_HeaderHovered] = ToImVec4(EditorStyle.HeaderHover);
		StyleColours[ImGuiCol_HeaderActive] = ToImVec4(EditorStyle.HeaderActive);

		// Buttons
		StyleColours[ImGuiCol_Button] = ToImVec4(EditorStyle.Button);
		StyleColours[ImGuiCol_ButtonHovered] = ToImVec4(EditorStyle.ButtonHoverd);
		StyleColours[ImGuiCol_ButtonActive] = ToImVec4(EditorStyle.ButtonActive);

		// Frame Background
		StyleColours[ImGuiCol_FrameBg] = ToImVec4(EditorStyle.FrameBackground);
		StyleColours[ImGuiCol_FrameBgHovered] = ToImVec4(EditorStyle.FrameHovered);
		StyleColours[ImGuiCol_FrameBgActive] = ToImVec4(EditorStyle.FrameActive);

		// Tabs
		StyleColours[ImGuiCol_Tab] = ToImVec4(EditorStyle.Tab);
		StyleColours[ImGuiCol_TabHovered] = ToImVec4(EditorStyle.TabHovered);
		StyleColours[ImGuiCol_TabActive] = ToImVec4(EditorStyle.TabActive);
		StyleColours[ImGuiCol_TabUnfocused] = ToImVec4(EditorStyle.TabUnfocused);
		StyleColours[ImGuiCol_TabUnfocusedActive] = ToImVec4(EditorStyle.TabUnfocusedActive);
		
		// Title
		StyleColours[ImGuiCol_TitleBg] = ToImVec4(EditorStyle.Title);
		StyleColours[ImGuiCol_TitleBgActive] = ToImVec4(EditorStyle.TitleActive);
		StyleColours[ImGuiCol_TitleBgCollapsed] = ToImVec4(EditorStyle.TitleCollapsed);
	}
}
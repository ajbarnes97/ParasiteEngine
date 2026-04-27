#include "pepch.h"
#include "ParasiteEngine/Core/Input.h"

#include "GLFW/glfw3.h"
#include "ParasiteEngine/Core/Application.h"


namespace Parasite
{
	bool CInput::IsKeyPressed(int InKeyCode)
	{
		GLFWwindow* Window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow());
		int State = glfwGetKey(Window, InKeyCode);
		return State == GLFW_PRESS || State == GLFW_REPEAT;
	}

	bool CInput::IsMousePressed(int InButton)
	{
		GLFWwindow* Window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow());
		int State = glfwGetMouseButton(Window, InButton);
		return State == GLFW_PRESS;
	}

	std::pair<float, float> CInput::GetMousePosition()
	{
		double MouseX, MouseY;
		GLFWwindow* Window = static_cast<GLFWwindow*>(CApplication::Get().GetWindow().GetNativeWindow());
		glfwGetCursorPos(Window, &MouseX, &MouseY);
		return { static_cast<float>(MouseX), static_cast<float>(MouseY) };
	}

	float CInput::GetMouseX()
	{
		auto [XPos, YPos] = GetMousePosition();
		return XPos;
	}

	float CInput::GetMouseY()
	{
		auto [XPos, YPos] = GetMousePosition();
		return YPos;
	}
}
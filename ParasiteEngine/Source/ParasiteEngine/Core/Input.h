#pragma once
#include "Core.h"


namespace Parasite
{
	class CInput
	{
	public:
		static bool IsKeyPressed(int InKeyCode);
		static bool IsMousePressed(int InButton);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
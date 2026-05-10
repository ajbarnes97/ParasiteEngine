#include "UIEditorUtils.h"

#include "ImGui/imgui.h"


namespace Parasite
{
	bool DrawColour(const char* InLabel, SColour& InColour, FOnColourChanged OnColourChanged)
	{
		ImVec4 Temp(InColour.R, InColour.G, InColour.B, InColour.A);

		if (ImGui::ColorEdit4(InLabel, &Temp.x))
		{
			InColour = SColour(Temp.x, Temp.y, Temp.z, Temp.w);

			if (OnColourChanged)
			{
				OnColourChanged(InColour);
			}
			return true;
		}
		return false;
	}
}
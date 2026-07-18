#include "UIEditorUtils.h"

#include "ParasiteEngine/Math/Colour.h"

#include "ImGui/imgui.h"


namespace Parasite
{
	namespace EditorUI
	{
		void DrawReflectedProperties(void* Instance, const SClassInfo* InInfo)
		{
			for (const SPropertyInfo& Property : InInfo->Properties)
			{
				std::any Value = Property.Get(Instance);

				if (Property.Type == std::type_index(typeid(float))) // todo: draw float ImGUI
				{
					float FloatValue = std::any_cast<float>(Value);
					if (ImGui::DragFloat(Property.Name.c_str(), &FloatValue, 0.01f))
					{
						Property.Set(Instance, FloatValue);
					}
				}
				else if (Property.Type == std::type_index(typeid(SColour)))// todo: draw colour ImGUI
				{
					SColour ColourValue = std::any_cast<SColour>(Value);
					if (ImGui::ColorEdit4(Property.Name.c_str(), &ColourValue.R, ImGuiColorEditFlags_NoInputs))
					{
						Property.Set(Instance, ColourValue);
					}
				}
				else
				{
					ImGui::Text("%s (unsupported reflected type)", Property.Name.c_str());
				}
			}
		}
	}
}
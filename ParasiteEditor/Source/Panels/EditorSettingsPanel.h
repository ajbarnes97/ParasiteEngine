#pragma once
#include "ParasiteEngine.h"

#include "ImGui/imgui.h"
#include "../Utils/UIEditorUtils.h"


namespace Parasite
{
	class CEditorSettingsPanel
	{
	public:
		CEditorSettingsPanel() = default;

		void ShowWindow(SEditorSettings* InSettings);

		void OnImGuiRender();

		void DrawCategoryAppearance();

	public:
		FOnColourChanged OnStyleChanged;
		std::function<void(const SEditorSettings*)> OnEditorSettingsApplied;

	private:
		bool bWindowOpen = false;
		SEditorSettings* EditorSettings;
	};
}
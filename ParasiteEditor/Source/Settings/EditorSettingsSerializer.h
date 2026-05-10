#pragma once
#include "ParasiteEngine/Math/Colour.h"
#include "EditorSettings.h"


namespace Parasite
{
	class CEditorSettingsSerializer
	{
	public:
		static void Save(const std::string& InFilepath, const SEditorSettings& Settings);
		static bool Load(const std::string& InFilepath, SEditorSettings& OutSettings);
	};
}
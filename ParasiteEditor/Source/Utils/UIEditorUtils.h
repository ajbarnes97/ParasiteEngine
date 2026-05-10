#pragma once
#include "ParasiteEngine/Math/Colour.h"


namespace Parasite
{
	using FOnColourChanged = std::function<void(const SColour&)>;

	bool DrawColour(const char* InLabel, SColour& InColour, FOnColourChanged OnColourChanged = nullptr);
}
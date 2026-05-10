#pragma once
#include "ParasiteEngine/Math/Colour.h"


namespace Parasite
{
	struct SEditorSettings
	{
	public:
		// Main
		SColour Background = SColour(0.1059f, 0.1098f, 0.1569f);
		SColour Border = SColour(0.0706f, 0.0745f, 0.1059f);

		// Headers
		SColour Header = SColour(0.0057805f, 0.063010f, 0.102242f);
		SColour HeaderHovered = SColour(0.034340f, 0.038204f, 0.061246f);
		SColour HeaderActive = SColour(0.057805f, 0.063010f, 0.102242f);

		// Buttons
		SColour Button = SColour(0.057805f, 0.063010f, 0.102242f);
		SColour ButtonHovered = SColour(0.22f, 0.231f, 0.325f);
		SColour ButtonActive = SColour(0.057805f, 0.063010f, 0.102242f);

		// Frame Background
		SColour FrameBackground = SColour(0.1373f, 0.1451f, 0.2f);
		SColour FrameHovered = SColour(0.1373f, 0.1451f, 0.2f);
		SColour FrameActive = SColour(0.1373f, 0.1451f, 0.2f);

		// Tabs
		SColour Tab = SColour(0.0706f, 0.0745f, 0.1059f);
		SColour TabHovered = SColour(0.0706f, 0.0745f, 0.1059f);
		SColour TabActive = SColour(0.0706f, 0.0745f, 0.1059f);
		SColour TabUnfocused = SColour(0.0706f, 0.0745f, 0.1059f);
		SColour TabUnfocusedActive = SColour(0.0706f, 0.0745f, 0.1059f);

		// Title
		SColour Title = SColour(0.1373f, 0.1451f, 0.2f);
		SColour TitleActive = SColour(0.1373f, 0.1451f, 0.2f);
		SColour TitleCollapsed = SColour(0.1373f, 0.1451f, 0.2f);
	};
}
#pragma once
#include "ParasiteEngine/Math/Colour.h"


namespace Parasite
{
	struct SEditorSettings
	{
	public:
		// Main
		SColour Background = SColour(0.105f, 0.110f, 0.145f);
		SColour Border = SColour(0.180f, 0.190f, 0.260f);

		// Headers
		SColour Header = SColour(0.160f, 0.120f, 0.260f);
		SColour HeaderHovered = SColour(0.220f, 0.180f, 0.340f);
		SColour HeaderActive = SColour(0.300f, 0.240f, 0.460f);

		// Buttons
		SColour Button = SColour(0.180f, 0.140f, 0.300f);
		SColour ButtonHovered = SColour(0.250f, 0.200f, 0.400f);
		SColour ButtonActive = SColour(0.340f, 0.270f, 0.520f);

		// Frame Background
		SColour FrameBackground = SColour(0.130f, 0.135f, 0.190f);
		SColour FrameHovered = SColour(0.170f, 0.175f, 0.240f);
		SColour FrameActive = SColour(0.210f, 0.220f, 0.300f);

		// Tabs
		SColour Tab = SColour(0.090f, 0.095f, 0.130f);
		SColour TabHovered = SColour(0.180f, 0.140f, 0.300f);
		SColour TabActive = SColour(0.250f, 0.200f, 0.400f);
		SColour TabUnfocused = SColour(0.080f, 0.085f, 0.115f);
		SColour TabUnfocusedActive = SColour(0.120f, 0.125f, 0.170f);

		// Title
		SColour Title = SColour(0.085f, 0.090f, 0.125f);
		SColour TitleActive = SColour(0.120f, 0.125f, 0.170f);
		SColour TitleCollapsed = SColour(0.070f, 0.075f, 0.105f);

		// Window / Background
		SColour ChildBackground = SColour(0.090f, 0.095f, 0.130f);
		SColour PopupBackground = SColour(0.115f, 0.120f, 0.165f);
		SColour BorderShadow = SColour(0.0f, 0.0f, 0.0f, 0.0f);
		SColour MenuBarBackground = SColour(0.085f, 0.090f, 0.125f);

		// Text
		SColour Text = SColour(0.940f, 0.940f, 0.890f);
		SColour TextDisabled = SColour(0.550f, 0.550f, 0.600f);
		SColour TextLink = SColour(0.400f, 0.700f, 1.000f);
		SColour TextSelectedBackground = SColour(0.320f, 0.250f, 0.500f, 0.8f);

		// Scrollbar
		SColour ScrollbarBackground = SColour(0.070f, 0.075f, 0.105f);
		SColour ScrollbarGrab = SColour(0.180f, 0.140f, 0.300f);
		SColour ScrollbarGrabHovered = SColour(0.250f, 0.200f, 0.400f);
		SColour ScrollbarGrabActive = SColour(0.340f, 0.270f, 0.520f);

		// Misc
		SColour CheckMark = SColour(0.630f, 0.380f, 0.950f);
		SColour SliderGrab = SColour(0.630f, 0.380f, 0.950f);
		SColour SliderGrabActive = SColour(0.760f, 0.520f, 1.000f);
		SColour InputTextCursor = SColour(1.0f, 1.0f, 1.0f);

		SColour TreeLines = SColour(0.350f, 0.360f, 0.500f);
		SColour DragDropTarget = SColour(1.000f, 0.250f, 0.600f);
		SColour DragDropTargetBackground = SColour(0.630f, 0.380f, 0.950f, 0.35f);
		SColour UnsavedMarker = SColour(1.000f, 0.550f, 0.200f);

		SColour ModalWindowDimBackground = SColour(0.0f, 0.0f, 0.0f, 0.45f);

		// Separator
		SColour Separator = SColour(0.260f, 0.270f, 0.380f);
		SColour SeparatorHovered = SColour(0.400f, 0.420f, 0.550f);
		SColour SeparatorActive = SColour(0.630f, 0.380f, 0.950f);

		// Resize Grip
		SColour ResizeGrip = SColour(0.250f, 0.200f, 0.400f);
		SColour ResizeGripHovered = SColour(0.630f, 0.380f, 0.950f);
		SColour ResizeGripActive = SColour(0.760f, 0.520f, 1.000f);

		// Additional Tabs
		SColour TabSelected = SColour(0.250f, 0.200f, 0.400f);
		SColour TabDimmed = SColour(0.080f, 0.085f, 0.115f);
		SColour TabDimmedSelected = SColour(0.160f, 0.120f, 0.260f);
		SColour TabDimmedSelectedOverline = SColour(0.630f, 0.380f, 0.950f);

		// Docking
		SColour DockingPreview = SColour(0.630f, 0.380f, 0.950f, 0.7f);
		SColour DockingEmptyBackground = SColour(0.090f, 0.095f, 0.130f);

		// Plot
		SColour PlotLines = SColour(0.400f, 0.700f, 1.000f);
		SColour PlotLinesHovered = SColour(1.000f, 0.250f, 0.600f);
		SColour PlotHistogram = SColour(0.630f, 0.380f, 0.950f);
		SColour PlotHistogramHovered = SColour(0.760f, 0.520f, 1.000f);

		// Table
		SColour TableHeaderBackground = SColour(0.085f, 0.090f, 0.125f);
		SColour TableBorderStrong = SColour(0.220f, 0.230f, 0.320f);
		SColour TableBorderLight = SColour(0.140f, 0.150f, 0.210f);
		SColour TableRowBackground = SColour(0.0f, 0.0f, 0.0f, 0.0f);
		SColour TableRowBackgroundAlt = SColour(1.0f, 1.0f, 1.0f, 0.02f);

		// Navigation
		SColour NavWindowingHighlight = SColour(0.630f, 0.380f, 0.950f);
		SColour NavWindowingDimBackground = SColour(0.0f, 0.0f, 0.0f, 0.45f);
		SColour NavCursor = SColour(0.630f, 0.380f, 0.950f);
	};
}
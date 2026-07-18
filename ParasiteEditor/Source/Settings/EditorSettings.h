#pragma once
#include "ParasiteEngine/Math/Colour.h"
#include "ReflectionCore.h"

#include "EditorSettings.generated.h"


namespace Parasite
{
	PE_CLASS()
	class SEditorSettings
	{
		PE_GENERATED_BODY()

	public:
		// Main
		PE_PROPERTY()
		SColour Background = SColour(0.105f, 0.110f, 0.145f);
		PE_PROPERTY()
		SColour Border = SColour(0.180f, 0.190f, 0.260f);

		// Headers
		PE_PROPERTY()
		SColour Header = SColour(0.160f, 0.120f, 0.260f);
		PE_PROPERTY()
		SColour HeaderHovered = SColour(0.220f, 0.180f, 0.340f);
		PE_PROPERTY()
		SColour HeaderActive = SColour(0.300f, 0.240f, 0.460f);

		// Buttons
		PE_PROPERTY()
		SColour Button = SColour(0.180f, 0.140f, 0.300f);
		PE_PROPERTY()
		SColour ButtonHovered = SColour(0.250f, 0.200f, 0.400f);
		PE_PROPERTY()
		SColour ButtonActive = SColour(0.340f, 0.270f, 0.520f);

		// Frame Background
		PE_PROPERTY()
		SColour FrameBackground = SColour(0.130f, 0.135f, 0.190f);
		PE_PROPERTY()
		SColour FrameHovered = SColour(0.170f, 0.175f, 0.240f);
		PE_PROPERTY()
		SColour FrameActive = SColour(0.210f, 0.220f, 0.300f);

		// Tabs
		PE_PROPERTY()
		SColour Tab = SColour(0.090f, 0.095f, 0.130f);
		PE_PROPERTY()
		SColour TabHovered = SColour(0.180f, 0.140f, 0.300f);
		PE_PROPERTY()
		SColour TabActive = SColour(0.250f, 0.200f, 0.400f);
		PE_PROPERTY()
		SColour TabUnfocused = SColour(0.080f, 0.085f, 0.115f);
		PE_PROPERTY()
		SColour TabUnfocusedActive = SColour(0.120f, 0.125f, 0.170f);

		// Title
		PE_PROPERTY()
		SColour Title = SColour(0.085f, 0.090f, 0.125f);
		PE_PROPERTY()
		SColour TitleActive = SColour(0.120f, 0.125f, 0.170f);
		PE_PROPERTY()
		SColour TitleCollapsed = SColour(0.070f, 0.075f, 0.105f);

		// Window / Background
		PE_PROPERTY()
		SColour ChildBackground = SColour(0.090f, 0.095f, 0.130f);
		PE_PROPERTY()
		SColour PopupBackground = SColour(0.115f, 0.120f, 0.165f);
		PE_PROPERTY()
		SColour BorderShadow = SColour(0.0f, 0.0f, 0.0f, 0.0f);
		PE_PROPERTY()
		SColour MenuBarBackground = SColour(0.085f, 0.090f, 0.125f);

		// Text
		PE_PROPERTY()
		SColour Text = SColour(0.940f, 0.940f, 0.890f);
		PE_PROPERTY()
		SColour TextDisabled = SColour(0.550f, 0.550f, 0.600f);
		PE_PROPERTY()
		SColour TextLink = SColour(0.400f, 0.700f, 1.000f);
		PE_PROPERTY()
		SColour TextSelectedBackground = SColour(0.320f, 0.250f, 0.500f, 0.8f);

		// Scrollbar
		PE_PROPERTY()
		SColour ScrollbarBackground = SColour(0.070f, 0.075f, 0.105f);
		PE_PROPERTY()
		SColour ScrollbarGrab = SColour(0.180f, 0.140f, 0.300f);
		PE_PROPERTY()
		SColour ScrollbarGrabHovered = SColour(0.250f, 0.200f, 0.400f);
		PE_PROPERTY()
		SColour ScrollbarGrabActive = SColour(0.340f, 0.270f, 0.520f);

		// Misc
		PE_PROPERTY()
		SColour CheckMark = SColour(0.630f, 0.380f, 0.950f);
		PE_PROPERTY()
		SColour SliderGrab = SColour(0.630f, 0.380f, 0.950f);
		PE_PROPERTY()
		SColour SliderGrabActive = SColour(0.760f, 0.520f, 1.000f);
		PE_PROPERTY()
		SColour InputTextCursor = SColour(1.0f, 1.0f, 1.0f);

		PE_PROPERTY()
		SColour TreeLines = SColour(0.350f, 0.360f, 0.500f);
		PE_PROPERTY()
		SColour DragDropTarget = SColour(1.000f, 0.250f, 0.600f);
		PE_PROPERTY()
		SColour DragDropTargetBackground = SColour(0.630f, 0.380f, 0.950f, 0.35f);
		PE_PROPERTY()
		SColour UnsavedMarker = SColour(1.000f, 0.550f, 0.200f);

		PE_PROPERTY()
		SColour ModalWindowDimBackground = SColour(0.0f, 0.0f, 0.0f, 0.45f);

		// Separator
		PE_PROPERTY()
		SColour Separator = SColour(0.260f, 0.270f, 0.380f);
		PE_PROPERTY()
		SColour SeparatorHovered = SColour(0.400f, 0.420f, 0.550f);
		PE_PROPERTY()
		SColour SeparatorActive = SColour(0.630f, 0.380f, 0.950f);

		// Resize Grip
		PE_PROPERTY()
		SColour ResizeGrip = SColour(0.250f, 0.200f, 0.400f);
		PE_PROPERTY()
		SColour ResizeGripHovered = SColour(0.630f, 0.380f, 0.950f);
		PE_PROPERTY()
		SColour ResizeGripActive = SColour(0.760f, 0.520f, 1.000f);

		// Additional Tabs
		PE_PROPERTY()
		SColour TabSelected = SColour(0.250f, 0.200f, 0.400f);
		PE_PROPERTY()
		SColour TabDimmed = SColour(0.080f, 0.085f, 0.115f);
		PE_PROPERTY()
		SColour TabDimmedSelected = SColour(0.160f, 0.120f, 0.260f);
		PE_PROPERTY()
		SColour TabDimmedSelectedOverline = SColour(0.630f, 0.380f, 0.950f);

		// Docking
		PE_PROPERTY()
		SColour DockingPreview = SColour(0.630f, 0.380f, 0.950f, 0.7f);
		PE_PROPERTY()
		SColour DockingEmptyBackground = SColour(0.090f, 0.095f, 0.130f);

		// Plot
		PE_PROPERTY()
		SColour PlotLines = SColour(0.400f, 0.700f, 1.000f);
		PE_PROPERTY()
		SColour PlotLinesHovered = SColour(1.000f, 0.250f, 0.600f);
		PE_PROPERTY()
		SColour PlotHistogram = SColour(0.630f, 0.380f, 0.950f);
		PE_PROPERTY()
		SColour PlotHistogramHovered = SColour(0.760f, 0.520f, 1.000f);

		// Table
		PE_PROPERTY()
		SColour TableHeaderBackground = SColour(0.085f, 0.090f, 0.125f);
		PE_PROPERTY()
		SColour TableBorderStrong = SColour(0.220f, 0.230f, 0.320f);
		PE_PROPERTY()
		SColour TableBorderLight = SColour(0.140f, 0.150f, 0.210f);
		PE_PROPERTY()
		SColour TableRowBackground = SColour(0.0f, 0.0f, 0.0f, 0.0f);
		PE_PROPERTY()
		SColour TableRowBackgroundAlt = SColour(1.0f, 1.0f, 1.0f, 0.02f);

		// Navigation
		PE_PROPERTY()
		SColour NavWindowingHighlight = SColour(0.630f, 0.380f, 0.950f);
		PE_PROPERTY()
		SColour NavWindowingDimBackground = SColour(0.0f, 0.0f, 0.0f, 0.45f);
		PE_PROPERTY()
		SColour NavCursor = SColour(0.630f, 0.380f, 0.950f);
	};
}
#pragma once

#include "ParasiteEngine/Core/Layer.h"

#include "ParasiteEngine/Events/MouseEvents.h"
#include "ParasiteEngine/Events/KeyEvent.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"

#include "ParasiteEngine/Math/Colour.h"

struct ImVec4;

namespace Parasite
{
	struct SEditorStyle
	{
	public:
		// Main
		SColour Background = SColour(0.1059f, 0.1098f, 0.1569f);
		SColour Border = SColour(0.0706f, 0.0745f, 0.1059f);

		// Headers
		SColour Header = SColour(0.0057805f, 0.063010f, 0.102242f);
		SColour HeaderHover = SColour(0.034340f, 0.038204f, 0.061246f);
		SColour HeaderActive = SColour(0.057805f, 0.063010f, 0.102242f);

		// Buttons
		SColour Button = SColour(0.057805f, 0.063010f, 0.102242f);
		SColour ButtonHoverd = SColour(0.0980f, 0.1020f, 0.1451f);
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

	class PARASITE_API CImGuiLayer : public CLayer
	{
	public:
		CImGuiLayer();
		~CImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(CEvent& InEvent) override;

		void Begin();
		void End();

		void SetBlockEvents(bool bInBlock) { bBlockEvents = bInBlock; };

	private:
		void ApplyEditorTheme();

		ImVec4 ToImVec4(const SColour& InColour);

	private:
		bool bBlockEvents = true;
		float Time = 0.0f;

		SEditorStyle EditorStyle;
	};
}
#pragma once
#include "ParasiteEngine.h"


namespace Parasite
{
	class CSceneHierarchyPanel
	{
	public:
		CSceneHierarchyPanel() = default;
		CSceneHierarchyPanel(const TSharedPtr<CScene>& InScene);
	
		void SetContext(const TSharedPtr<CScene>& InScene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(CEntity InEntity);

	private:
		TSharedPtr<CScene> Context;
		CEntity SelectionContext;
	};
}
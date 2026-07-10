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

		inline CEntity GetSelectionContext() const { return SelectionContext; }
		void SetSelectedContext(CEntity InEntity);

	private:
		void DrawEntityNode(CEntity InEntity);
		void DrawComponents(CEntity InEntity);

	private:
		TSharedPtr<CScene> Context;
		CEntity SelectionContext;
	};
}
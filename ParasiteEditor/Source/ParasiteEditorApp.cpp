#include "ParasiteEngine.h"
#include "ParasiteEngine/Core/EntryPoint.h"

#include "ParasiteEngine/Core/Layer.h"
#include "ParasiteEngine/Events/Event.h"

#include "EditorLayer.h"


namespace Parasite
{
	class CParasiteEditorApp : public CApplication
	{
	public:
		CParasiteEditorApp()
			: CApplication("Parasite Editor")
		{
			PushLayer(new CEditorLayer());
		}

		~CParasiteEditorApp()
		{
		}
	};

	CApplication* CreateApplication()
	{
		return new CParasiteEditorApp();
	}
}
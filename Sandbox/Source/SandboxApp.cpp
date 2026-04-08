#include "ParasiteEngine.h"

#include "ParasiteEngine/Layer.h"
#include "ParasiteEngine/Events/Event.h"

#include "ImGui/imgui.h"


class CTestLayer : public Parasite::CLayer
{
public:
	CTestLayer()
		: CLayer("Example Layer")
	{
	}

	virtual void OnUpdate() override
	{
	}

	virtual void OnEvent(Parasite::CEvent& InEvent)
	{
	}
};


class CSandbox : public Parasite::CApplication
{
public:
	CSandbox()
	{
		PushLayer(new CTestLayer());
	}

	~CSandbox()
	{
	}
};

Parasite::CApplication* Parasite::CreateApplication()
{
	return new CSandbox();
}
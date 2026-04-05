#include "ParasiteEngine.h"

#include "ParasiteEngine/Layer.h"
#include "ParasiteEngine/Events/Event.h"


class CTestLayer : public Parasite::CLayer
{
public:
	CTestLayer()
		: CLayer("Example Layer")
	{
	}

	virtual void OnUpdate() override
	{
		PE_LOG("CTestLayer::Update");
	}

	virtual void OnEvent(Parasite::CEvent& InEvent)
	{
		PE_LOG("{0}", InEvent.ToString());
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
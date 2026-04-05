#include "pepch.h"

#include "LayerStack.h"


namespace Parasite
{
	CLayerStack::CLayerStack()
	{
		LayerInsert = Layers.begin();
	}

	CLayerStack::~CLayerStack()
	{
		for (CLayer* Layer : Layers)
		{
			delete Layer;
		}
	}

	void CLayerStack::PushLayer(CLayer* InLayer)
	{
		LayerInsert = Layers.emplace(LayerInsert, InLayer);
	}

	void CLayerStack::PopLayer(CLayer* InLayer)
	{
		auto It = std::find(Layers.begin(), Layers.end(), InLayer);
		if (It != Layers.end())
		{
			Layers.erase(It);
			LayerInsert--;
		}
	}

	void CLayerStack::PushOverlay(CLayer* InOverlay)
	{
		Layers.emplace_back(InOverlay);
	}

	void CLayerStack::PopOverlay(CLayer* InOverlay)
	{
		auto It = std::find(Layers.begin(), Layers.end(), InOverlay);
		if (It != Layers.end())
		{
			Layers.erase(It);
		}
	}
}
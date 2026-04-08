#include "pepch.h"

#include "LayerStack.h"


namespace Parasite
{
	CLayerStack::CLayerStack()
	{
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
		Layers.emplace(Layers.begin() + LayerInsertIndex, InLayer);
		LayerInsertIndex++;
		InLayer->OnAttach();
	}

	void CLayerStack::PopLayer(CLayer* InLayer)
	{
		auto It = std::find(Layers.begin(), Layers.end(), InLayer);
		if (It != Layers.end())
		{
			Layers.erase(It);
			LayerInsertIndex--;
		}
	}

	void CLayerStack::PushOverlay(CLayer* InOverlay)
	{
		Layers.emplace_back(InOverlay);
		InOverlay->OnAttach();
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
#pragma once

#include "ParasiteEngine/Core.h"
#include "Layer.h"


namespace Parasite
{
	class PARASITE_API CLayerStack
	{
	public:
		CLayerStack();
		~CLayerStack();

		void PushLayer(CLayer* InLayer);
		void PopLayer(CLayer* InLayer);

		void PushOverlay(CLayer* InOverlay);
		void PopOverlay(CLayer* InOverlay);

		std::vector<CLayer*>::iterator begin() { return Layers.begin(); }
		std::vector<CLayer*>::iterator end() { return Layers.end(); }

	private:
		std::vector<CLayer*> Layers;
		unsigned int LayerInsertIndex = 0;
	};
}
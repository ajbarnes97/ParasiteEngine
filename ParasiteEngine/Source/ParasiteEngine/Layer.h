#pragma once

#include "ParasiteEngine/Core.h"
#include "ParasiteEngine/Events/Event.h"


namespace Parasite
{
	class PARASITE_API CLayer
	{
	public:
		CLayer(const std::string& InLayerName = "Layer");
		virtual ~CLayer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(CEvent& InEvent) {};

		inline const std::string& GetName() const { return DebugName; }

	private:
		std::string DebugName;
	};
}
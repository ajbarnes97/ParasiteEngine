#pragma once

#include "ParasiteEngine/Layer.h"

#include "ParasiteEngine/Events/MouseEvents.h"
#include "ParasiteEngine/Events/KeyEvent.h"
#include "ParasiteEngine/Events/ApplicationEvent.h"


namespace Parasite
{
	class PARASITE_API CImGuiLayer : public CLayer
	{
	public:
		CImGuiLayer();
		~CImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float Time = 0.0f;
	};
}
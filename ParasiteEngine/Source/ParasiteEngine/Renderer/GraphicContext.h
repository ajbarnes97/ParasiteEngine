#pragma once


namespace Parasite
{
	class CGraphicContext
	{
	public:
		virtual ~CGraphicContext() = default;
		
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
};
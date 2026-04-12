#pragma once
#include "ParasiteEngine/Renderer/RendererAPI.h"


namespace Parasite
{
	class COpenGLRendererAPI : public CRendererAPI
	{
	public:
		virtual void SetClearColour(const glm::vec4& InColour) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const TSharedPtr<CVertexArray>& InVertexArray) override;
	};
}
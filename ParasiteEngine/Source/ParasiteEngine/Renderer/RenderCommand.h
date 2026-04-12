#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"

namespace Parasite
{
	class CRenderCommand
	{
	public:
		inline static void DrawIndexed(const TSharedPtr<CVertexArray>& InVertexArray)
		{
			RendererAPI->DrawIndexed(InVertexArray);
		}

		inline static void SetClearColour(const glm::vec4& InColour)
		{
			RendererAPI->SetClearColour(InColour);
		}

		inline static void Clear()
		{
			RendererAPI->Clear();
		}

	private:
		static CRendererAPI* RendererAPI;
	};
}
#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"


namespace Parasite
{
	class CRendererAPI
	{
	public:
		enum class EAPI
		{
			None = 0,
			OpenGL,
		};

	public:
		virtual void SetClearColour(const glm::vec4& InColour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<CVertexArray>& InVertexArray) = 0;

		inline static EAPI GetRendererAPI() { return RendererAPI; }

	private:
		static EAPI RendererAPI;
	};
}
#pragma once
#include "Texture.h"

#include "glm/ext/vector_float2.hpp"


namespace Parasite
{
	class CSubTexture2D
	{
	public:
		CSubTexture2D(const TSharedPtr<CTexture2D>& InTexture, const glm::vec2& InMinBound, const glm::vec2& InMaxBound);

		const inline TSharedPtr<CTexture2D> GetTexture() const { return Texture; }
		const inline glm::vec2* GetTexCoords() const { return TexCoords; }
	

		static TSharedPtr<CSubTexture2D> CreateFromCoords(const TSharedPtr<CTexture2D>& InTexture, const glm::vec2& InCoords, const glm::vec2& InCellSize, const glm::vec2& InSpriteSize = { 1, 1 });
	private:
		TSharedPtr<CTexture2D> Texture;

		glm::vec2 TexCoords[4];
	};
}
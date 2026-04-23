#include "pepch.h"
#include "ParasiteEngine/Renderer/SubTexture2D.h"


namespace Parasite
{
	CSubTexture2D::CSubTexture2D(const TSharedPtr<CTexture2D>& InTexture, const glm::vec2& InMinBound, const glm::vec2& InMaxBound)
		: Texture(InTexture)
	{
		TexCoords[0] = { InMinBound.x, InMinBound.y };
		TexCoords[1] = { InMaxBound.x, InMinBound.y };
		TexCoords[2] = { InMaxBound.x, InMaxBound.y };
		TexCoords[3] = { InMinBound.x, InMaxBound.y };
	}

	TSharedPtr<CSubTexture2D> CSubTexture2D::CreateFromCoords(const TSharedPtr<CTexture2D>& InTexture, const glm::vec2& InCoords, const glm::vec2& InCellSize, const glm::vec2& InSpriteSize)
	{
		glm::vec2 MinBounds = { (InCoords.x * InCellSize.x) / InTexture->GetWidth(), (InCoords.y * InCellSize.y) / InTexture->GetHeight() };
		glm::vec2 MaxBounds = { ((InCoords.x + InSpriteSize.x) * InCellSize.x) / InTexture->GetWidth(), ((InCoords.y + InSpriteSize.y) * InCellSize.y) / InTexture->GetHeight() };
		return MakeShared<CSubTexture2D>(InTexture, MinBounds, MaxBounds);
	}
}
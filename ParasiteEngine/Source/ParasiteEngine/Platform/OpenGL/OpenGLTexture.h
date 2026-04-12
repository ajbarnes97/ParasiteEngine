#pragma once

#include "ParasiteEngine/Renderer/Texture.h"

namespace Parasite
{
	class COpenGLTexture2D : public CTexture2D
	{
	public:
		COpenGLTexture2D(const std::string& InPath);
		virtual ~COpenGLTexture2D();

		inline virtual uint32_t GetWidth() const override { return Width; }
		inline virtual uint32_t GetHeight() const override { return Height; }

		virtual void Bind(uint32_t InSlot) const override;

	private:
		std::string Path;

		uint32_t Width;
		uint32_t Height;
		uint32_t RendererID;
	};
}
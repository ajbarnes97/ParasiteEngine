#pragma once
#include "ParasiteEngine/Renderer/Texture.h"

#include "GLAD/glad.h"


namespace Parasite
{
	class COpenGLTexture2D : public CTexture2D
	{
	public:
		COpenGLTexture2D(const std::string& InPath);
		COpenGLTexture2D(uint32_t InWidth, uint32_t InHeight);
		virtual ~COpenGLTexture2D();

		virtual void SetData(void* InData, uint32_t InSize) override;

		inline virtual uint32_t GetWidth() const override { return Width; }
		inline virtual uint32_t GetHeight() const override { return Height; }

		virtual void Bind(uint32_t InSlot) const override;

		virtual bool operator==(const CTexture& InOther) const override { return RendererID == ((COpenGLTexture2D&)InOther).RendererID; };

	private:
		std::string Path;

		uint32_t Width;
		uint32_t Height;
		uint32_t RendererID;
		GLenum InternalFormat, Format;
	};
}
#include "pepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"


namespace Parasite
{
	COpenGLTexture2D::COpenGLTexture2D(const std::string& InPath)
		: Path(InPath)
	{
		int LoadedWidth, LoadedHeight, Channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* Data = stbi_load(Path.c_str(), &LoadedWidth, &LoadedHeight, &Channels, 0);
		PE_CORE_ASSERT(Data, "Failed to load image.");

		Width = LoadedWidth;
		Height = LoadedHeight;

		GLenum OpenGLFormat, DataFormat = 0;
		if (Channels == 4)
		{
			OpenGLFormat = GL_RGBA8;
			DataFormat = GL_RGBA;
		}
		else if (Channels == 3)
		{
			OpenGLFormat = GL_RGB8;
			DataFormat = GL_RGB;
		}
		else
		{
			PE_CORE_ASSERT(false, "Format not supported.");
		}

		InternalFormat = OpenGLFormat;
		Format = DataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
		glTextureStorage2D(RendererID, 1, OpenGLFormat, Width, Height);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, Data);

		stbi_image_free(Data);
	}

	COpenGLTexture2D::COpenGLTexture2D(uint32_t InWidth, uint32_t InHeight)
		:Width(InWidth), Height(InHeight)
	{
		InternalFormat = GL_RGBA8;
		Format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
		glTextureStorage2D(RendererID, 1, InternalFormat, Width, Height);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	COpenGLTexture2D::~COpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererID);
	}

	void COpenGLTexture2D::SetData(void* InData, uint32_t InSize)
	{
		uint32_t BitsPerPixel = Format == GL_RGBA ? 4 : 3;
		PE_CORE_ASSERT(InSize == Width * Height * BitsPerPixel, "Data must be entire texture.");
		glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, InData);
	}

	void COpenGLTexture2D::Bind(uint32_t InSlot) const
	{
		glBindTextureUnit(InSlot, RendererID);
	}
}
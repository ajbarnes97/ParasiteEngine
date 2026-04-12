#include "pepch.h"
#include "OpenGLTexture.h"

#include "GLAD/glad.h"
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

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
		glTextureStorage2D(RendererID, 1, GL_RGB8, Width, Height);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, Data);

		stbi_image_free(Data);
	}

	COpenGLTexture2D::~COpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererID);
	}

	void COpenGLTexture2D::Bind(uint32_t InSlot) const
	{
		glBindTextureUnit(InSlot, RendererID);
	}
}
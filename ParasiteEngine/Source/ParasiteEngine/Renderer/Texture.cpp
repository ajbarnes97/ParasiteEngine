#include "pepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "ParasiteEngine/Platform/OpenGL/OpenGLTexture.h"

namespace Parasite
{
	TSharedPtr<CTexture2D> CTexture2D::Create(const std::string& InFilePath)
	{
		switch (CRenderer::GetRendererAPI())
		{
		case CRendererAPI::EAPI::None: return nullptr;
		case CRendererAPI::EAPI::OpenGL: return MakeShared<COpenGLTexture2D>(InFilePath);
		}
		return nullptr;
	}

	TSharedPtr<CTexture2D> CTexture2D::Create(uint32_t InWidth, uint32_t InHeight)
	{
		switch (CRenderer::GetRendererAPI())
		{
		case CRendererAPI::EAPI::None: return nullptr;
		case CRendererAPI::EAPI::OpenGL: return MakeShared<COpenGLTexture2D>(InWidth, InHeight);
		}
		return nullptr;
	}
}
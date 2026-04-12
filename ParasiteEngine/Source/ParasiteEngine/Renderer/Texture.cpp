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
		case CRendererAPI::EAPI::OpenGL: return std::make_shared<COpenGLTexture2D>(InFilePath);
		}
		return nullptr;
	}
}
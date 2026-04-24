#include "pepch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "ParasiteEngine/Platform/OpenGL/OpenGLFrameBuffer.h"


namespace Parasite
{
	TSharedPtr<CFrameBuffer> CFrameBuffer::Create(const SFrameBufferSpecification& InSpec)
	{
		switch (CRenderer::GetRendererAPI())
		{
			case CRendererAPI::EAPI::None: return nullptr;
			case CRendererAPI::EAPI::OpenGL: return MakeShared<COpenGLFrameBuffer>(InSpec);
		}
		return nullptr;
	}
}
#include "pepch.h"
#include "RenderCommand.h"

#include "ParasiteEngine/Platform/OpenGL/OpenGLRendererAPI.h"


namespace Parasite
{
	CRendererAPI* CRenderCommand::RendererAPI = new COpenGLRendererAPI();
}
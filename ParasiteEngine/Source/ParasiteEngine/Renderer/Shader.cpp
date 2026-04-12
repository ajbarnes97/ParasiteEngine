#include "pepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"


namespace Parasite
{
	CShader* CShader::Create(const std::string& InVertexSource, const std::string& InFragmentSource)
	{
		switch (CRenderer::GetRendererAPI())
		{
		case CRendererAPI::EAPI::None: return nullptr;
		case CRendererAPI::EAPI::OpenGL: return new COpenGLShader(InVertexSource, InFragmentSource);
		}
		return nullptr;
	}
}
#include "pepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "ParasiteEngine/Platform/OpenGL/OpenGLBuffer.h"


namespace Parasite
{
	void CBufferLayout::CalculateOffsetAndStride()
	{
		uint32_t Offset = 0;
		Stride = 0;
		for (SBufferElement& CurrentElement : Elements)
		{
			CurrentElement.Offset = Offset;
			Offset += CurrentElement.Size;
			Stride += CurrentElement.Size;
		}
	}


	CVertexBuffer* CVertexBuffer::Create(float* InVertices, uint32_t InSize)
	{
		switch (CRenderer::GetRendererAPI())
		{
			case CRendererAPI::EAPI::None: return nullptr;
			case CRendererAPI::EAPI::OpenGL: return new COpenGLVertexBuffer(InVertices, InSize);
		}
		return nullptr;
	}


	CIndexBuffer* CIndexBuffer::Create(uint32_t* InIndices, uint32_t InSize)
	{
		switch (CRenderer::GetRendererAPI())
		{
			case CRendererAPI::EAPI::None: return nullptr;
			case CRendererAPI::EAPI::OpenGL: return new COpenGLIndexBuffer(InIndices, InSize);
		}
		return nullptr;
	}
}
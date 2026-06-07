#include "pepch.h"
#include "OpenGLVertexArray.h"

#include "Glad/glad.h"


namespace Parasite
{
	static GLenum ShaderDataTypeToOpenGLBaseType(EShaderDataType InType)
	{
		switch (InType)
		{
			case Parasite::EShaderDataType::Float:		return GL_FLOAT;
			case Parasite::EShaderDataType::Float2:		return GL_FLOAT;
			case Parasite::EShaderDataType::Float3:		return GL_FLOAT;
			case Parasite::EShaderDataType::Float4:		return GL_FLOAT;
			case Parasite::EShaderDataType::Mat3:		return GL_FLOAT;
			case Parasite::EShaderDataType::Mat4:		return GL_FLOAT;
			case Parasite::EShaderDataType::Int:		return GL_INT;
			case Parasite::EShaderDataType::Int2:		return GL_INT;
			case Parasite::EShaderDataType::Int3:		return GL_INT;
			case Parasite::EShaderDataType::Int4:		return GL_INT;
			case Parasite::EShaderDataType::Bool:		return GL_BOOL;
		}

		PE_ASSERT(false, "Failed to find OpenGL type for shader data type.");
		return 0;
	}

	COpenGLVertexArray::COpenGLVertexArray()
	{
		glCreateVertexArrays(1, &RendererID);
	}

	COpenGLVertexArray::~COpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &RendererID);
	}

	void COpenGLVertexArray::Bind() const
	{
		glBindVertexArray(RendererID);
	}

	void COpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void COpenGLVertexArray::AddVertexBuffer(const TSharedPtr<CVertexBuffer>& InVertexBuffer)
	{
		glBindVertexArray(RendererID);
		InVertexBuffer->Bind();

		PE_ASSERT(InVertexBuffer->GetLayout().GetElements().size() > 0, "Vertex Buffer has no layout.");

		uint32_t Index = 0;
		const auto& Layout = InVertexBuffer->GetLayout();
		for (const auto& Element : Layout)
		{
			switch (Element.Type)
			{
			case EShaderDataType::Float: // Fall through
			case EShaderDataType::Float2: // Fall through
			case EShaderDataType::Float3: // Fall through
			case EShaderDataType::Float4: // Fall through
			{
				glEnableVertexAttribArray(Index);
				glVertexAttribPointer(Index, Element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(Element.Type),
					Element.bNormalized ? GL_TRUE : GL_FALSE,
					Layout.GetStride(),
					(const void*)(uintptr_t)Element.Offset);
				Index++;
				break;
			}
			case EShaderDataType::Int: // Fall through
			case EShaderDataType::Int2: // Fall through
			case EShaderDataType::Int3: // Fall through
			case EShaderDataType::Int4: // Fall through
			case EShaderDataType::Bool: // Fall through
			{
				glEnableVertexAttribArray(Index);
				glVertexAttribIPointer(Index, Element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(Element.Type),
					Layout.GetStride(),
					(const void*)(uintptr_t)Element.Offset);
				Index++;
				break;
			}
			default:
			{
				PE_CORE_ASSERT(false, "Unknow shader type!");
				break;
			}
			}
		}

		VertexBuffers.push_back(InVertexBuffer);
	}

	void COpenGLVertexArray::SetIndexBuffer(const TSharedPtr<CIndexBuffer>& InIndexBuffer)
	{
		glBindVertexArray(RendererID);
		InIndexBuffer->Bind();

		IndexBuffer = InIndexBuffer;
	}
}
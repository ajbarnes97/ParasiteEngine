#pragma once
#include "ParasiteEngine/Renderer/VertexArray.h"


namespace Parasite
{
	class COpenGLVertexArray : public CVertexArray
	{
	public:
		COpenGLVertexArray();
		virtual ~COpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const TSharedPtr<CVertexBuffer>& InVertexBuffer) override;
		virtual void SetIndexBuffer(const TSharedPtr<CIndexBuffer>& InIndexBuffer) override;
	
		virtual const std::vector<TSharedPtr<CVertexBuffer>>& GetVertexBuffers() const { return VertexBuffers; }
		virtual const TSharedPtr<CIndexBuffer>& GetIndexBuffer() const { return IndexBuffer; };

	private:
		uint32_t RendererID;

		std::vector<TSharedPtr<CVertexBuffer>> VertexBuffers;
		TSharedPtr<CIndexBuffer> IndexBuffer;
	};
}
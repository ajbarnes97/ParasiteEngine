#pragma once

#include "memory"
#include "ParasiteEngine/Renderer/Buffer.h"


namespace Parasite
{
	class CVertexArray
	{
	public:
		virtual ~CVertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const TSharedPtr<CVertexBuffer>& InVertexBuffer) = 0;
		virtual void SetIndexBuffer(const TSharedPtr<CIndexBuffer>& InIndexBuffer) = 0;

		virtual const std::vector<TSharedPtr<CVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const TSharedPtr<CIndexBuffer>& GetIndexBuffer() const = 0;

		static CVertexArray* Create();
	};
}
#pragma once
#include "ParasiteEngine/Renderer/FrameBuffer.h"

namespace Parasite
{
	class COpenGLFrameBuffer : public CFrameBuffer
	{
	public:
		COpenGLFrameBuffer(const SFrameBufferSpecification& InSpec);
		virtual ~COpenGLFrameBuffer();

		virtual inline const SFrameBufferSpecification& GetSpecification() const override { return Specification; }
		virtual inline uint32_t GetColourAttachmentRendererID() const override { return ColourAttachment; };

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		uint32_t RendererID;
		uint32_t ColourAttachment, DepthAttachment;
		SFrameBufferSpecification Specification;
	};
}
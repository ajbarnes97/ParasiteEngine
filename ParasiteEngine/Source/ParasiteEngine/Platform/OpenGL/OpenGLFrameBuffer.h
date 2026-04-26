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

		virtual void Resize(uint32_t InWindowSizeX, uint32_t InWindowSizeY) override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		uint32_t RendererID = 0;
		uint32_t ColourAttachment = 0, DepthAttachment = 0;
		SFrameBufferSpecification Specification;
	};
}
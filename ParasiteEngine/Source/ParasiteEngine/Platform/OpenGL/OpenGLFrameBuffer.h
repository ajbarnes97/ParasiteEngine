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
		virtual inline uint32_t GetColourAttachmentRendererID(uint32_t InIndex = 0) const override { return ColourAttachments[InIndex]; };

		virtual int ReadPixel(uint32_t InAttachmentIndex, int InX, int InY) override;

		virtual void ClearAttachment(uint32_t InAttachmentIndex, const int InValue) override;

		virtual void Resize(uint32_t InWindowSizeX, uint32_t InWindowSizeY) override;

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		uint32_t RendererID = 0;
		SFrameBufferSpecification Specification;

		std::vector<SFrameBufferTextureSpecification> ColourAttachmentSpecifications;
		SFrameBufferTextureSpecification DepthAttachmentSpecification;

		std::vector<uint32_t> ColourAttachments;
		uint32_t DepthAttachment;
	};
}
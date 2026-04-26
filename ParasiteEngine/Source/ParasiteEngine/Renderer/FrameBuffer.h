#pragma once

#include "ParasiteEngine/Core/Core.h"

namespace Parasite
{
	struct SFrameBufferSpecification
	{
	public:
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool bSwapChainTarget = false;
	};

	class CFrameBuffer
	{
	public:
		virtual inline const SFrameBufferSpecification& GetSpecification() const = 0;
		virtual inline uint32_t GetColourAttachmentRendererID() const = 0;

		virtual void Resize(uint32_t InWindowSizeX, uint32_t InWindowSizeY) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static TSharedPtr<CFrameBuffer> Create(const SFrameBufferSpecification& InSpec);
	};
}
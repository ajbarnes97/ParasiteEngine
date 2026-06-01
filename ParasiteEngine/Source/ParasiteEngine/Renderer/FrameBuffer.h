#pragma once

#include "ParasiteEngine/Core/Core.h"

namespace Parasite
{
	enum class EFrameBufferTextureFormat
	{
		None = 0,
		RGBA8, // Colour
		DEPTH24STENCIL8, // Depth/Stencil
		
		Depth = DEPTH24STENCIL8, // Defaults
	};

	struct SFrameBufferTextureSpecification
	{
	public:
		SFrameBufferTextureSpecification() = default;
		SFrameBufferTextureSpecification(EFrameBufferTextureFormat InFormat)
			: TextureFormat(InFormat)
		{ }
	
		EFrameBufferTextureFormat TextureFormat = EFrameBufferTextureFormat::None;
		//todo: setup filtering/wrap
	};

	struct SFrameBufferAttachmentSpecification
	{
	public:
		SFrameBufferAttachmentSpecification() = default;
		SFrameBufferAttachmentSpecification(std::initializer_list<SFrameBufferTextureSpecification> InAttachments)
			: Attachments(InAttachments)
		{ }

		std::vector<SFrameBufferTextureSpecification> Attachments;
	};

	struct SFrameBufferSpecification
	{
	public:
		uint32_t Width, Height;
		SFrameBufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool bSwapChainTarget = false;
	};

	class CFrameBuffer
	{
	public:
		virtual ~CFrameBuffer() = default;

		virtual inline const SFrameBufferSpecification& GetSpecification() const = 0;
		virtual inline uint32_t GetColourAttachmentRendererID(uint32_t InIndex = 0) const = 0;

		virtual void Resize(uint32_t InWindowSizeX, uint32_t InWindowSizeY) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static TSharedPtr<CFrameBuffer> Create(const SFrameBufferSpecification& InSpec);
	};
}
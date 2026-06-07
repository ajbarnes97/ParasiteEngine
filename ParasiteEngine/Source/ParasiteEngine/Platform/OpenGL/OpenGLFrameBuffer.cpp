#include "pepch.h"
#include "OpenGLFrameBuffer.h"

#include "glad/glad.h"


namespace Parasite
{
	namespace Utils
	{
		static GLenum GetTextureTarget(bool bInMultisampled)
		{
			return bInMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void BindTexture(bool bInMultisampledm, uint32_t InID)
		{
			glBindTexture(GetTextureTarget(bInMultisampledm), InID);
		}

		static bool IsDepthFormat(EFrameBufferTextureFormat InFormat)
		{
			switch (InFormat)
			{
			case EFrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}

		static void CreateTextures(bool bInMultisampled, uint32_t* OutID, uint32_t InCount)
		{
			glCreateTextures(GetTextureTarget(bInMultisampled), InCount, OutID);
		}

		static void AttachColourTexture(uint32_t InID, uint32_t InSamples, GLenum InInternalFormat, GLenum InFormat, uint32_t InWidth, uint32_t InHeight, int InIndex)
		{
			bool bMultisample = InSamples > 1;
			if (bMultisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, InSamples, InInternalFormat, InWidth, InHeight, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, InInternalFormat, InWidth, InHeight, 0, InFormat, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + InIndex, GetTextureTarget(bMultisample), InID, 0);
		}

		static void AttachDepthTexture(uint32_t InID, int InSamples, GLenum InFormat, GLenum InAttachmentType, uint32_t InWidth, uint32_t InHeight)
		{
			bool bMultisample = InSamples > 1;
			if (bMultisample)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, InSamples, InFormat, InWidth, InHeight, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, InFormat, InWidth, InHeight);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, InAttachmentType, GetTextureTarget(bMultisample), InID, 0);
		}

		static GLenum ParasiteTextureFormatToGL(EFrameBufferTextureFormat InFormat)
		{
			switch (InFormat)
			{
			case EFrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case EFrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			return 0;
		}
	}


	COpenGLFrameBuffer::COpenGLFrameBuffer(const SFrameBufferSpecification& InSpec)
		: Specification(InSpec)
	{
		for (auto Format : Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(Format.TextureFormat))
			{
				ColourAttachmentSpecifications.emplace_back(Format);
			}
			else
			{
				DepthAttachmentSpecification = Format.TextureFormat;
			}
		}

		Invalidate();
	}

	COpenGLFrameBuffer::~COpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &RendererID);
		glDeleteTextures(static_cast<GLsizei>(ColourAttachments.size()), ColourAttachments.data());
		glDeleteTextures(1, &DepthAttachment);
	}

	int COpenGLFrameBuffer::ReadPixel(uint32_t InAttachmentIndex, int InX, int InY)
	{
		PE_CORE_ASSERT(InAttachmentIndex < ColourAttachments.size(), "Index exceeds number of attachements.");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + InAttachmentIndex);
		int PixelData = -1;
		glReadPixels(InX, InY, 1, 1, GL_RED_INTEGER, GL_INT, &PixelData);
		return PixelData;
	}

	void COpenGLFrameBuffer::ClearAttachment(uint32_t InAttachmentIndex, const int InValue)
	{
		auto& Spec = ColourAttachmentSpecifications[InAttachmentIndex];
		GLenum ValueFormat = Utils::ParasiteTextureFormatToGL(Spec.TextureFormat);
		glClearTexImage(ColourAttachments[InAttachmentIndex], 0, ValueFormat, GL_INT, &InValue);
	}

	void COpenGLFrameBuffer::Resize(uint32_t InWindowSizeX, uint32_t InWindowSizeY)
	{
		if (InWindowSizeX == 0 || InWindowSizeY == 0)
		{
			PE_CORE_WARN("Attempting to resize window to an invalid size.");
			return;
		}

		Specification.Width = InWindowSizeX;
		Specification.Height = InWindowSizeY;

		Invalidate();
	}

	void COpenGLFrameBuffer::Invalidate()
	{
		if (RendererID)
		{
			glDeleteFramebuffers(1, &RendererID);
			glDeleteTextures(static_cast<GLsizei>(ColourAttachments.size()), ColourAttachments.data());
			glDeleteTextures(1, &DepthAttachment);

			ColourAttachments.clear();
			DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, RendererID);

		const bool bMultisample = Specification.Samples > 1;

		// Attachments
		if (ColourAttachmentSpecifications.size())
		{
			ColourAttachments.resize(ColourAttachmentSpecifications.size());
			Utils::CreateTextures(bMultisample, ColourAttachments.data(), static_cast<uint32_t>(ColourAttachments.size()));

			for (size_t Index = 0; Index < ColourAttachments.size(); Index++)
			{
				Utils::BindTexture(bMultisample, ColourAttachments[Index]);
				switch (ColourAttachmentSpecifications[Index].TextureFormat)
				{
				case EFrameBufferTextureFormat::RGBA8:
				{
					Utils::AttachColourTexture(ColourAttachments[Index], Specification.Samples, GL_RGBA8, GL_RGBA, Specification.Width, Specification.Height, static_cast<int>(Index));
					break;
				}
				case EFrameBufferTextureFormat::RED_INTEGER:
				{
					Utils::AttachColourTexture(ColourAttachments[Index], Specification.Samples, GL_R32I, GL_RED_INTEGER, Specification.Width, Specification.Height, static_cast<int>(Index));
					break;
				}
				}
			}
		}

		if (DepthAttachmentSpecification.TextureFormat != EFrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(bMultisample, &DepthAttachment, 1);
			Utils::BindTexture(bMultisample, DepthAttachment);
			switch (DepthAttachmentSpecification.TextureFormat)
			{
			case EFrameBufferTextureFormat::DEPTH24STENCIL8:
			{
				Utils::AttachDepthTexture(DepthAttachment, Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, Specification.Width, Specification.Height);
				break;
			}
			}
		}
		
		if (ColourAttachments.size() > 1)
		{
			PE_CORE_ASSERT(ColourAttachments.size() <= 4, "Only supprot max colour attachments 4.");
			GLenum Buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(static_cast<GLsizei>(ColourAttachments.size()), Buffers);
		}
		else if (ColourAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		PE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void COpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
		glViewport(0, 0, Specification.Width, Specification.Height);

	}

	void COpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
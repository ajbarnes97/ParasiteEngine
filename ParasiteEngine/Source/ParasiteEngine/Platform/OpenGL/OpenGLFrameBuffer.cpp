#include "pepch.h"
#include "OpenGLFrameBuffer.h"

#include "glad/glad.h"


namespace Parasite
{
	COpenGLFrameBuffer::COpenGLFrameBuffer(const SFrameBufferSpecification& InSpec)
		: Specification(InSpec)
	{
		Invalidate();
	}

	COpenGLFrameBuffer::~COpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &RendererID);
		glDeleteTextures(1, &ColourAttachment);
		glDeleteTextures(1, &DepthAttachment);
	}

	void COpenGLFrameBuffer::Resize(uint32_t InWindowSizeX, uint32_t InWindowSizeY)
	{
		if (InWindowSizeX == 0 || InWindowSizeY == 0)
		{
			PE_WARN("Attempting to resize window to an invalid size.");
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
			glDeleteTextures(1, &ColourAttachment);
			glDeleteTextures(1, &DepthAttachment);
		}

		glCreateFramebuffers(1, &RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &ColourAttachment);
		glBindTexture(GL_TEXTURE_2D, ColourAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Specification.Width, Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColourAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Specification.Width, Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);

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
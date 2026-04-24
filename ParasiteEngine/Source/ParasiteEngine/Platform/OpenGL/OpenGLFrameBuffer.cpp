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
	}

	void COpenGLFrameBuffer::Invalidate()
	{
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
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Specification.Width, Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);

		PE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void COpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
	}

	void COpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
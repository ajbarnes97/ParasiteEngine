#include "pepch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"


namespace Parasite
{
	void COpenGLRendererAPI::SetClearColour(const glm::vec4& InColour)
	{
		glClearColor(InColour.r, InColour.g, InColour.b, InColour.a);
	}

	void COpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void COpenGLRendererAPI::DrawIndexed(const std::shared_ptr<CVertexArray>& InVertexArray)
	{
		glDrawElements(GL_TRIANGLES, InVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
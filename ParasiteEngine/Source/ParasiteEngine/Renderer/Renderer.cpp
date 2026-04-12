#include "pepch.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"


namespace Parasite
{
	CRenderer::SSceneData* CRenderer::SceneData = new CRenderer::SSceneData();

	void CRenderer::BeginScene(COrthographicCamera& InCamera)
	{
		SceneData->ViewProjectionMatrix = InCamera.GetViewProjectionMatrix();
	}

	void CRenderer::EndScene()
	{

	}

	void CRenderer::Submit(const std::shared_ptr<CShader> InShader, const std::shared_ptr<CVertexArray>& InVertexArray, const glm::mat4& InTransform)
	{
		InShader->Bind();
		std::dynamic_pointer_cast<COpenGLShader>(InShader)->UploadUniformMat4("u_ViewProjection", SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<COpenGLShader>(InShader)->UploadUniformMat4("u_Transform", InTransform);

		InVertexArray->Bind();
		CRenderCommand::DrawIndexed(InVertexArray);
	}
}
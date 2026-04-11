#include "pepch.h"
#include "Renderer.h"
#include "RenderCommand.h"


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

	void CRenderer::Submit(const std::shared_ptr<CShader> InShader, const std::shared_ptr<CVertexArray>& InVertexArray)
	{
		InShader->Bind();

		InShader->UploadUniformMat4("u_ViewProjection", SceneData->ViewProjectionMatrix);

		InVertexArray->Bind();
		CRenderCommand::DrawIndexed(InVertexArray);
	}
}
#pragma once
#include "RendererAPI.h"

#include "Camera.h"
#include "Shader.h"


namespace Parasite
{
	class CRenderer
	{
	public:
		static void BeginScene(COrthographicCamera& InCamera);
		static void EndScene();

		static void Submit(const std::shared_ptr<CShader> InShader, const std::shared_ptr<CVertexArray>& InVertexArray);

		inline static CRendererAPI::EAPI GetRendererAPI() { return CRendererAPI::GetRendererAPI(); }
	
	private:
		struct SSceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SSceneData* SceneData;
	};
}

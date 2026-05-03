#pragma once
#include "RendererAPI.h"

#include "Camera.h"
#include "Shader.h"


namespace Parasite
{
	class CRenderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t InWidth, uint32_t InHeight);

		static void BeginScene(CCamera& InCamera, const glm::mat4& InTransform);
		static void EndScene();

		static void Submit(const TSharedPtr<CShader> InShader, const TSharedPtr<CVertexArray>& InVertexArray, const glm::mat4& InTransform = glm::mat4(1.0f));

		inline static CRendererAPI::EAPI GetRendererAPI() { return CRendererAPI::GetRendererAPI(); }
	
	private:
		struct SSceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SSceneData* SceneData;
	};
}

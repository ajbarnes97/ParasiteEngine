#pragma once
#include "ParasiteEngine/Renderer/Shader.h"


namespace Parasite
{
	class COpenGLShader : public CShader
	{
	public:
		COpenGLShader(const std::string& InVertexSource, const std::string& InFragmentSource);
		~COpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& InName, const int InValue);
		
		void UploadUniformFloat(const std::string& InName, const float InValue);
		void UploadUniformFloat2(const std::string& InName, const glm::vec2 InValues);
		void UploadUniformFloat3(const std::string& InName, const glm::vec3 InValues);
		void UploadUniformFloat4(const std::string& InName, const glm::vec4 InValues);

		void UploadUniformMat3(const std::string& InName, const glm::mat3& InUniform);
		void UploadUniformMat4(const std::string& InName, const glm::mat4& InUniform);

	private:
		uint32_t RendererID;
	};
}
#pragma once
#include "ParasiteEngine/Renderer/Shader.h"

//todo: remove 
typedef unsigned int GLenum;

namespace Parasite
{
	class COpenGLShader : public CShader
	{
	public:
		COpenGLShader(const std::string& InFilepath);
		COpenGLShader(const std::string& InName, const std::string& InVertexSource, const std::string& InFragmentSource);
		~COpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetFloat4(const std::string& InName, const glm::vec4& InValue) override;
		virtual void SetFloat3(const std::string& InName, const glm::vec3& InValue) override;
		virtual void SetFloat2(const std::string& InName, const glm::vec2& InValue) override;
		virtual void SetFloat(const std::string& InName, const float InValue) override;

		virtual void SetMat4(const std::string& InName, const glm::mat4& InValue) override;

		virtual void SetInt(const std::string& InName, int InValue) override;
		virtual void SetIntArray(const std::string& InName, int* InValues,  uint32_t InCount) override;

		virtual const std::string& GetName() const override { return Name; };

		void UploadUniformInt(const std::string& InName, const int InValue);
		void UploadUniformIntArray(const std::string& InName, const int* InValues, uint32_t InCount);
		
		void UploadUniformFloat(const std::string& InName, const float InValue);
		void UploadUniformFloat2(const std::string& InName, const glm::vec2 InValues);
		void UploadUniformFloat3(const std::string& InName, const glm::vec3 InValues);
		void UploadUniformFloat4(const std::string& InName, const glm::vec4 InValues);

		void UploadUniformMat3(const std::string& InName, const glm::mat3& InUniform);
		void UploadUniformMat4(const std::string& InName, const glm::mat4& InUniform);

	private:
		std::string ReadFile(const std::string& InFilepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& InSource);
		void Complie(std::unordered_map<GLenum, std::string>& InShaderSources);

	private:
		uint32_t RendererID;
		std::string Name;
	};
}
#pragma once

#include "string"

#include "glm/glm.hpp"
#include "unordered_map"


namespace Parasite
{
	class CShader
	{
	public:
		virtual ~CShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetFloat4(const std::string& InName, const glm::vec4& InValue) = 0;
		virtual void SetFloat3(const std::string& InName, const glm::vec3& InValue) = 0;
		virtual void SetFloat2(const std::string& InName, const glm::vec2& InValue) = 0;
		virtual void SetFloat(const std::string& InName, const float InValue) = 0;

		virtual void SetMat4(const std::string& InName, const glm::mat4& InValue) = 0;
		
		virtual void SetInt(const std::string& InName, int InValue) = 0;
		virtual void SetIntArray(const std::string& InName, int* InValues, uint32_t InCount) = 0;

		virtual const std::string& GetName() const = 0;

		static TSharedPtr<CShader> Create(const std::string& InFilePath);
		static TSharedPtr<CShader> Create(const std::string& InName, const std::string& InVertexSource, const std::string& InFragmentSource);
	};


	class CShaderLibirary
	{
	public:
		void Add(const std::string& InName, const TSharedPtr<CShader>&InShader);
		void Add(const TSharedPtr<CShader>& InShader);

		TSharedPtr<CShader> Load(const std::string& InFilepath);
		TSharedPtr<CShader> Load(const std::string& InName, const std::string& InFilepath);
		TSharedPtr<CShader> Load(const std::string& InName, const std::string& InVertexSource, const std::string& InFragmentSource);

		TSharedPtr<CShader> Get(const std::string& InName);

	private:
		std::unordered_map<std::string, TSharedPtr<CShader>> Shaders;
	};
}
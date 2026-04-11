#pragma once

#include "string"
#include "glm/glm.hpp"


namespace Parasite
{
	class CShader
	{
	public:
		CShader(const std::string& InVertexSource, const std::string& InFragmentSource);
		~CShader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& InName, const glm::mat4& InUniform);

	private:
		uint32_t RendererID;
	};
}
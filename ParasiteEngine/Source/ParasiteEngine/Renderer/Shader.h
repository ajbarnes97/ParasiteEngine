#pragma once

#include "string"
#include "glm/glm.hpp"


namespace Parasite
{
	class CShader
	{
	public:
		virtual ~CShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static CShader* Create(const std::string& InVertexSource, const std::string& InFragmentSource);
	};
}
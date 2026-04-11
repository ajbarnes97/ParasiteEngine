#include "pepch.h"
#include "Shader.h"

#include "GLAD/glad.h"
#include "glm/gtc/type_ptr.hpp"


namespace Parasite
{

	CShader::CShader(const std::string& InVertexSource, const std::string& InFragmentSource)
	{
		// Read our shaders into the appropriate buffers
		std::string vertexSource = InVertexSource;
		std::string fragmentSource = InFragmentSource;

		// Create an empty vertex shader handle
		RendererID = glCreateShader(GL_VERTEX_SHADER);
		GLuint vertexShader = RendererID;

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			PE_CORE_ERROR("{0}", infoLog.data());
			PE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			PE_CORE_ERROR("{0}", infoLog.data());
			PE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			PE_CORE_ERROR("{0}", infoLog.data());
			PE_ASSERT(false, "Shader link failure!");
			return;
		}

		RendererID = program;

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	CShader::~CShader()
	{
		glDeleteProgram(RendererID);
	}

	void CShader::Bind() const
	{
		glUseProgram(RendererID);
	}

	void CShader::Unbind() const
	{
		glUseProgram(0);
	}

	void CShader::UploadUniformMat4(const std::string& InName, const glm::mat4& InUniform)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());

		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(InUniform));
	}
}
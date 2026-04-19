#include "pepch.h"

#include "OpenGLShader.h"
#include "ParasiteEngine/Renderer/Renderer.h"
#include "ParasiteEngine/Core/Core.h"

#include "fstream"
#include "GLAD/glad.h"
#include "glm/gtc/type_ptr.hpp"


namespace Parasite
{
	static GLenum ShaderTypeFromString(const std::string& InString)
	{
		if (InString == "vertex") return GL_VERTEX_SHADER;
		else if (InString == "pixel" || InString == "fragment") return GL_FRAGMENT_SHADER;

		PE_CORE_ASSERT(false, "Unknow shader type!");
		return 0;
	}

	COpenGLShader::COpenGLShader(const std::string& InFilepath)
	{
		std::string Source = ReadFile(InFilepath);
		auto ShaderSources = PreProcess(Source);
		Complie(ShaderSources);
	
		// Extract name from file path
		auto LastSlash = InFilepath.find_last_of("/\\");
		LastSlash = LastSlash == std::string::npos ? 0 : LastSlash + 1;
		auto LastDot = InFilepath.rfind(".");
		auto Count = LastDot == std::string::npos ? InFilepath.size() - LastSlash : LastDot - LastSlash;
		Name = InFilepath.substr(LastSlash, Count);
	}

	COpenGLShader::COpenGLShader(const std::string& InName, const std::string& InVertexSource, const std::string& InFragmentSource)
		:Name(InName)
	{
		std::unordered_map<GLenum, std::string> Sources;
		Sources[GL_VERTEX_SHADER] = InVertexSource;
		Sources[GL_FRAGMENT_SHADER] = InFragmentSource;
		Complie(Sources);
	}

	COpenGLShader::~COpenGLShader()
	{

	}

	void COpenGLShader::Bind() const
	{
		glUseProgram(RendererID);
	}

	void COpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void COpenGLShader::SetFloat4(const std::string& InName, const glm::vec4& InValue)
	{
		UploadUniformFloat4(InName, InValue);
	}

	void COpenGLShader::SetFloat3(const std::string& InName, const glm::vec3& InValue)
	{
		UploadUniformFloat3(InName, InValue);
	}

	void COpenGLShader::SetFloat2(const std::string& InName, const glm::vec2& InValue)
	{
		UploadUniformFloat2(InName, InValue);
	}

	void COpenGLShader::SetFloat(const std::string& InName, const float InValue)
	{
		UploadUniformFloat(InName, InValue);
	}

	void COpenGLShader::SetMat4(const std::string& InName, const glm::mat4& InValue)
	{
		UploadUniformMat4(InName, InValue);
	}

	void COpenGLShader::SetInt(const std::string& InName, int InValue)
	{
		UploadUniformInt(InName, InValue);
	}

	void COpenGLShader::UploadUniformInt(const std::string& InName, const int InValue)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniform1i(Location, InValue);
	}

	void COpenGLShader::UploadUniformFloat(const std::string& InName, const float InValue)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniform1f(Location, InValue);
	}

	void COpenGLShader::UploadUniformFloat2(const std::string& InName, const glm::vec2 InValues)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniform2f(Location, InValues.x, InValues.y);
	}

	void COpenGLShader::UploadUniformFloat3(const std::string& InName, const glm::vec3 InValues)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniform3f(Location, InValues.x, InValues.y, InValues.z);
	}

	void COpenGLShader::UploadUniformFloat4(const std::string& InName, const glm::vec4 InValues)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniform4f(Location, InValues.x, InValues.y, InValues.z, InValues.w);
	}

	void COpenGLShader::UploadUniformMat3(const std::string& InName, const glm::mat3& InUniform)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniformMatrix3fv(Location, 1, GL_FALSE, glm::value_ptr(InUniform));
	}

	void COpenGLShader::UploadUniformMat4(const std::string& InName, const glm::mat4& InUniform)
	{
		GLint Location = glGetUniformLocation(RendererID, InName.c_str());
		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(InUniform));
	}

	std::string COpenGLShader::ReadFile(const std::string& InFilepath)
	{
		std::string Result;
		std::ifstream in(InFilepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			Result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&Result[0], Result.size());
			in.close();
		}
		else
		{
			PE_CORE_ERROR("Could not open file '{0}'", InFilepath);
		}
		return Result;
	}

	std::unordered_map<GLenum, std::string> COpenGLShader::PreProcess(const std::string& InSource)
	{
		std::unordered_map<GLenum, std::string> ShaderSources;

		const char* TypeToken = "#type";
		size_t TypeTokenLength = strlen(TypeToken);
		size_t Position = InSource.find(TypeToken, 0);
		
		while (Position != std::string::npos)
		{
			size_t EndOfLine = InSource.find_first_of("\r\n", Position);
			PE_CORE_ASSERT(EndOfLine != std::string::npos, "Syntax Error.");
			
			size_t Begin = Position + TypeTokenLength + 1;
			std::string Type = InSource.substr(Begin, EndOfLine - Begin);
			PE_CORE_ASSERT(ShaderTypeFromString(Type), "Invalid shader type.");

			size_t NextLinePosition = InSource.find_first_of("\r\n", EndOfLine);
			Position = InSource.find(TypeToken, NextLinePosition);
			ShaderSources[ShaderTypeFromString(Type)] = 
					InSource.substr(NextLinePosition, 
							Position - (NextLinePosition == std::string::npos ? InSource.size() -1 : NextLinePosition));
		}
		return ShaderSources;
	}

	void COpenGLShader::Complie(std::unordered_map<GLenum, std::string>& InShaderSources)
	{
		GLuint Program = glCreateProgram();
		std::vector<GLenum> GLShaderIDs;
		GLShaderIDs.reserve(InShaderSources.size());

		// Create and attach given shaders
		for (auto& [Type, Source] : InShaderSources)
		{
			GLuint Shader = glCreateShader(Type);

			const GLchar* SourceCStr = Source.c_str();
			glShaderSource(Shader, 1, &SourceCStr, 0);
			glCompileShader(Shader);

			GLint isCompiled = 0;
			glGetShaderiv(Shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(Shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(Shader);

				PE_CORE_ERROR("{0}", infoLog.data());
				PE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(Program, Shader);
			GLShaderIDs.push_back(Type);
		}

		glLinkProgram(Program);

		GLint isLinked = 0;
		glGetProgramiv(Program, GL_LINK_STATUS, (int*)&isLinked);

		// Upon failing to link - clean up created shaders
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(Program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(Program);

			for (GLenum Type : GLShaderIDs)
			{
				glDeleteShader(Type);
			}

			PE_CORE_ERROR("{0}", infoLog.data());
			PE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (GLenum Type : GLShaderIDs)
		{
			glDetachShader(Program, Type);
		}
		
		RendererID = Program;
	}
}
#include "ParasiteEngine.h"

#include "ParasiteEngine/Layer.h"
#include "ParasiteEngine/Events/Event.h"

#include "ImGui/imgui.h"
#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"

using namespace Parasite;

class CTestLayer : public Parasite::CLayer
{
public:
	CTestLayer()
		: CLayer("Example Layer")
			, OrthoCamera(-2.0f, 2.0f, -2.0f, 2.0f)
			, CameraPosition(0.0f)
	{
		VertexArray.reset(CVertexArray::Create());

		float Vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f,			0.8f, 0.2f, 0.8f, 1.f,
			 0.5f, -0.5f, 0.0f,			0.2f, 0.3f, 0.8f, 1.f,
			 0.0f, 0.5f, 0.0f,			0.8f, 0.8f, 0.2f, 1.f,
		};

		TSharedPtr<CVertexBuffer> VertexBuffer;
		VertexBuffer.reset(CVertexBuffer::Create(Vertices, sizeof(Vertices)));
		CBufferLayout BufferLayout =
		{
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float4, "a_Color" },
		};
		VertexBuffer->SetLayout(BufferLayout);
		VertexArray->AddVertexBuffer(VertexBuffer);

		uint32_t Indices[3] = { 0, 1, 2 };
		TSharedPtr<CIndexBuffer> IndexBuffer;
		IndexBuffer.reset(CIndexBuffer::Create(Indices, 3));
		VertexArray->SetIndexBuffer(IndexBuffer);

		SquareVertexArray.reset(CVertexArray::Create());

		float SquareVertices[5 * 4] =
		{
			-0.5f,-0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		};

		TSharedPtr<CVertexBuffer> SquareVertexBuffer;
		SquareVertexBuffer.reset(CVertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		SquareVertexBuffer->SetLayout({
				{ EShaderDataType::Float3, "a_Position" },
				{ EShaderDataType::Float2, "a_TexCoord" },
			});
		SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);

		uint32_t SqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		TSharedPtr<CIndexBuffer> SqaureIndexBuffer;
		SqaureIndexBuffer.reset(CIndexBuffer::Create(SqaureIndices, sizeof(SqaureIndices) / sizeof(uint32_t)));
		SquareVertexArray->SetIndexBuffer(SqaureIndexBuffer);

		std::string VertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string FragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position* 0.5 + 0.5, 1.0);
				color = v_Color;	
			}
		)";

		std::string FlatColourVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string FlatColourFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			uniform vec3 u_Colour;

			void main()
			{
				color = vec4(u_Colour, 1.0f);	
			}
		)";

		std::string TextureVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string TextureFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		Shader.reset(CShader::Create(VertexSource, FragmentSource));
		TextureShader.reset(CShader::Create(TextureVertexSource, TextureFragmentSource));
		FlatColourShader.reset(CShader::Create(FlatColourVertexSource, FlatColourFragmentSource));

		//Texture = CTexture2D::Create("E:/Projects/ParasiteEngine/Assets/Textures/Screenshot.png");

		std::dynamic_pointer_cast<COpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<COpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	virtual void OnUpdate(CTimestep InTimestep) override
	{
		const float Speed = CameraSpeed *InTimestep;
		const float RotSpeed = RotationSpeed *InTimestep;
		if (CInput::IsKeyPressed(PE_KEY_D))
		{
			CameraPosition.x += Speed;
		}
		if (CInput::IsKeyPressed(PE_KEY_A))
		{
			CameraPosition.x -= Speed;
		}
		if (CInput::IsKeyPressed(PE_KEY_UP))
		{
			CameraRotation += RotSpeed;
		}
		if (CInput::IsKeyPressed(PE_KEY_DOWN))
		{
			CameraRotation -= RotSpeed;
		}

		CRenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		CRenderCommand::Clear();

		OrthoCamera.SetPosition(CameraPosition);
		OrthoCamera.SetRotation(CameraRotation);

		CRenderer::BeginScene(OrthoCamera);

		glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<COpenGLShader>(FlatColourShader)->Bind();
		std::dynamic_pointer_cast<COpenGLShader>(FlatColourShader)->UploadUniformFloat3("u_Colour", SqaureColour);

		for (int Y = 0; Y < 20; Y++)
		{
			for (int X = 0; X < 20; X++)
			{
				glm::vec3 Position = glm::vec3(X * 0.11f, Y * 0.11f, 0.0f);
				glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) * Scale;
				CRenderer::Submit(FlatColourShader, SquareVertexArray, Transform);
			}
		}
		
		Texture->Bind();
		CRenderer::Submit(TextureShader, SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//CRenderer::Submit(Shader, VertexArray);

		CRenderer::EndScene();
	}

	virtual void OnEvent(Parasite::CEvent& InEvent)
	{
	}

	virtual void  OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Colour:", glm::value_ptr(SqaureColour));

		ImGui::End();
	}

private:
	TSharedPtr<CShader> Shader;
	TSharedPtr<CVertexArray> VertexArray;

	TSharedPtr<CShader> FlatColourShader, TextureShader;
	TSharedPtr<CVertexArray> SquareVertexArray;
	TSharedPtr<CTexture2D> Texture;

	COrthographicCamera OrthoCamera;
	glm::vec3 CameraPosition = {0.0f, 0.0f, 0.0f};
	float CameraSpeed = 1.5f;
	float CameraRotation = 0.0f;
	float RotationSpeed = 180.0f;

	glm::vec3 SqaureColour = { 0.2f, 0.3f, 0.8f };
};


class CSandbox : public Parasite::CApplication
{
public:
	CSandbox()
	{
		PushLayer(new CTestLayer());
	}

	~CSandbox()
	{
	}
};

Parasite::CApplication* Parasite::CreateApplication()
{
	return new CSandbox();
}
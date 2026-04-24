#include "ParasiteEngine.h"
#include "ParasiteEngine/Core/EntryPoint.h"

#include "ParasiteEngine/Core/Layer.h"
#include "ParasiteEngine/Events/Event.h"

#include "ImGui/imgui.h"
#include "ParasiteEngine/Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Sandbox2D.h"

using namespace Parasite;


class CTestLayer : public Parasite::CLayer
{
public:
	CTestLayer() : CLayer("Example Layer")
		, OrthoCamera(1.7777f, true)
	{
		VertexArray = CVertexArray::Create();

		float Vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f,			0.8f, 0.2f, 0.8f, 1.f,
			 0.5f, -0.5f, 0.0f,			0.2f, 0.3f, 0.8f, 1.f,
			 0.0f, 0.5f, 0.0f,			0.8f, 0.8f, 0.2f, 1.f,
		};

		TSharedPtr<CVertexBuffer> VertexBuffer = CVertexBuffer::Create(Vertices, sizeof(Vertices));
		CBufferLayout BufferLayout =
		{
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float4, "a_Color" },
		};
		VertexBuffer->SetLayout(BufferLayout);
		VertexArray->AddVertexBuffer(VertexBuffer);

		uint32_t Indices[3] = { 0, 1, 2 };
		TSharedPtr<CIndexBuffer> IndexBuffer = CIndexBuffer::Create(Indices, 3);
		VertexArray->SetIndexBuffer(IndexBuffer);

		SquareVertexArray = CVertexArray::Create();

		float SquareVertices[5 * 4] =
		{
			-0.5f,-0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		};

		TSharedPtr<CVertexBuffer> SquareVertexBuffer = CVertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
		SquareVertexBuffer->SetLayout({
				{ EShaderDataType::Float3, "a_Position" },
				{ EShaderDataType::Float2, "a_TexCoord" },
			});
		SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);

		uint32_t SqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		TSharedPtr<CIndexBuffer> SqaureIndexBuffer = CIndexBuffer::Create(SqaureIndices, sizeof(SqaureIndices) / sizeof(uint32_t));
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

		auto TextureShader = ShaderLibirary.Load("Assets/Shaders/Texture.glsl");
		auto TriangleShader = ShaderLibirary.Load("VertexColourTriangle", VertexSource, FragmentSource);
		auto FlatColourShader = ShaderLibirary.Load("FlatColour", FlatColourVertexSource, FlatColourFragmentSource);

		Texture = CTexture2D::Create("Assets/Textures/Checkerboard.png");
		Texture2 = CTexture2D::Create("Assets/Textures/Checkerboard2.png");

		std::dynamic_pointer_cast<COpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<COpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnUpdate(CTimestep InTimestep) override
	{
		OrthoCamera.OnUpdate(InTimestep);

		CRenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		CRenderCommand::Clear();

		CRenderer::BeginScene(OrthoCamera.GetCamera());

		glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto FlatColourShader = ShaderLibirary.Get("FlatColour");

		std::dynamic_pointer_cast<COpenGLShader>(FlatColourShader)->Bind();
		std::dynamic_pointer_cast<COpenGLShader>(FlatColourShader)->UploadUniformFloat3("u_Colour", SqaureColour);

		for (int Y = 0; Y < 20; Y++)
		{
			for (int X = 0; X < 20; X++)
			{
				glm::vec3 Position = glm::vec3(X * 0.11f, Y * 0.11f, -1.0f);
				glm::mat4 Transform = glm::translate(glm::mat4(1.0f), Position) * Scale;
				CRenderer::Submit(FlatColourShader, SquareVertexArray, Transform);
			}
		}
		
		auto TextureShader = ShaderLibirary.Get("Texture");
		Texture->Bind();
		CRenderer::Submit(TextureShader, SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Texture2->Bind();
		CRenderer::Submit(TextureShader, SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		auto TriangleShader = ShaderLibirary.Get("VertexColourTriangle");
		CRenderer::Submit(TriangleShader, VertexArray);

		CRenderer::EndScene();
	}

	virtual void OnEvent(Parasite::CEvent& InEvent)
	{
		OrthoCamera.OnEvent(InEvent);
	}

private:
	CShaderLibirary ShaderLibirary;
	TSharedPtr<CVertexArray> VertexArray;
	TSharedPtr<CVertexArray> SquareVertexArray;
	TSharedPtr<CTexture2D> Texture, Texture2;

	COrthographicCameraController OrthoCamera;

	glm::vec3 SqaureColour = { 0.2f, 0.3f, 0.8f };
};


class CSandbox : public Parasite::CApplication
{
public:
	CSandbox()
	{
		//PushLayer(new CTestLayer());
		PushLayer(new CSandbox2D());
	}

	~CSandbox()
	{
	}
};

Parasite::CApplication* Parasite::CreateApplication()
{
	return new CSandbox();
}
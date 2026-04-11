#include "ParasiteEngine.h"

#include "ParasiteEngine/Layer.h"
#include "ParasiteEngine/Events/Event.h"

#include "ImGui/imgui.h"

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

		std::shared_ptr<CVertexBuffer> VertexBuffer;
		VertexBuffer.reset(CVertexBuffer::Create(Vertices, sizeof(Vertices)));
		CBufferLayout BufferLayout =
		{
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float4, "a_Color" },
		};
		VertexBuffer->SetLayout(BufferLayout);
		VertexArray->AddVertexBuffer(VertexBuffer);

		uint32_t Indices[3] = { 0, 1, 2 };
		std::shared_ptr<CIndexBuffer> IndexBuffer;
		IndexBuffer.reset(CIndexBuffer::Create(Indices, 3));
		VertexArray->SetIndexBuffer(IndexBuffer);

		SquareVertexArray.reset(CVertexArray::Create());

		float SquareVertices[3 * 4] =
		{
			-0.75f,		-0.75f, 0.0f,
			 0.75f,		-0.75f, 0.0f,
			 0.75f,		 0.75f, 0.0f,
			-0.75f,		 0.75f, 0.0f,
		};

		std::shared_ptr<CVertexBuffer> SquareVertexBuffer;
		SquareVertexBuffer.reset(CVertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		SquareVertexBuffer->SetLayout({
				{ EShaderDataType::Float3, "a_Position" },
			});
		SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);

		uint32_t SqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<CIndexBuffer> SqaureIndexBuffer;
		SqaureIndexBuffer.reset(CIndexBuffer::Create(SqaureIndices, sizeof(SqaureIndices) / sizeof(uint32_t)));
		SquareVertexArray->SetIndexBuffer(SqaureIndexBuffer);

		std::string VertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		std::string VertexSource2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string FragmentSource2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);	
			}
		)";

		Shader.reset(new CShader(VertexSource, FragmentSource));
		BlueShader.reset(new CShader(VertexSource2, FragmentSource2));
	}

	virtual void OnUpdate() override
	{
		if (CInput::IsKeyPressed(PE_KEY_D))
		{
			CameraPosition.x += CameraSpeed;
		}
		if (CInput::IsKeyPressed(PE_KEY_A))
		{
			CameraPosition.x -= CameraSpeed;
		}
		if (CInput::IsKeyPressed(PE_KEY_UP))
		{
			CameraRotation += RotationSpeed;
		}
		if (CInput::IsKeyPressed(PE_KEY_DOWN))
		{
			CameraRotation -= RotationSpeed;
		}

		CRenderCommand::SetClearColour(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		CRenderCommand::Clear();

		OrthoCamera.SetPosition(CameraPosition);
		OrthoCamera.SetRotation(CameraRotation);

		CRenderer::BeginScene(OrthoCamera);

		CRenderer::Submit(BlueShader, SquareVertexArray);
		CRenderer::Submit(Shader, VertexArray);

		CRenderer::EndScene();
	}

	virtual void OnEvent(Parasite::CEvent& InEvent)
	{
	}

private:
	std::shared_ptr<CShader> Shader;
	std::shared_ptr<CVertexArray> VertexArray;

	std::shared_ptr<CShader> BlueShader;
	std::shared_ptr<CVertexArray> SquareVertexArray;

	COrthographicCamera OrthoCamera;
	glm::vec3 CameraPosition = {0.0f, 0.0f, 0.0f};
	float CameraSpeed = 0.1f;
	float CameraRotation = 0.0f;
	float RotationSpeed = 1.0f;
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
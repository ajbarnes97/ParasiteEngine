#include "pepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/ext/matrix_transform.hpp"


namespace Parasite
{
	struct SRenderer2DData
	{
		TSharedPtr<CVertexArray> QuadVertexArray;
		TSharedPtr<CShader> TextureShader;
		TSharedPtr<CTexture2D> WhiteTexture;
	};
	static SRenderer2DData* Data;


	void CRenderer2D::Init()
	{
		Data = new SRenderer2DData();

		Data->QuadVertexArray = CVertexArray::Create();

		float SquareVertices[5 * 4] =
		{
			-0.5f,-0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		};

		TSharedPtr<CVertexBuffer> SquareVertexBuffer;
		SquareVertexBuffer.reset(CVertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		SquareVertexBuffer->SetLayout(
			{
				{ EShaderDataType::Float3, "a_Position" },
				{ EShaderDataType::Float2, "a_TexCoord" },
			}
		);
		Data->QuadVertexArray->AddVertexBuffer(SquareVertexBuffer);

		uint32_t SqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		TSharedPtr<CIndexBuffer> SqaureIndexBuffer;
		SqaureIndexBuffer.reset(CIndexBuffer::Create(SqaureIndices, sizeof(SqaureIndices) / sizeof(uint32_t)));
		Data->QuadVertexArray->SetIndexBuffer(SqaureIndexBuffer);

		Data->WhiteTexture = CTexture2D::Create(1, 1);
		uint32_t TextureData = 0xffffffff;
		Data->WhiteTexture->SetData(&TextureData, sizeof(TextureData));

		Data->TextureShader = CShader::Create("Assets/Shaders/Texture.glsl");
		Data->TextureShader->Bind();
		Data->TextureShader->SetInt("u_Texture", 0);
	}

	void CRenderer2D::Shutdown()
	{
		delete Data;
	}

	void CRenderer2D::BeginScene(const CCamera& InCamera)
	{
		Data->TextureShader->Bind();
		Data->TextureShader->SetMat4("u_ViewProjection", InCamera.GetViewProjectionMatrix());
	}

	void CRenderer2D::EndScene()
	{

	}

	void CRenderer2D::DrawQuad(const glm::vec2& InPosition, const glm::vec2& InSize, const glm::vec4 InColour)
	{
		DrawQuad({ InPosition.x, InPosition.y, 0.0f }, InSize, InColour);
	}

	void CRenderer2D::DrawQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const glm::vec4 InColour)
	{
		Data->TextureShader->SetFloat4("u_Colour", InColour);
		Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		Data->WhiteTexture->Bind();

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition) 
			* glm::scale(glm::mat4(1.0f), {InSize.x, InSize.y, 1.0f});
		Data->TextureShader->SetMat4("u_Transform", Transform);

		Data->QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(Data->QuadVertexArray);
	}

	void CRenderer2D::DrawQuad(const glm::vec2& InPosition, const glm::vec2& InSize, const TSharedPtr<CTexture>& InTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		DrawQuad({ InPosition.x, InPosition.y, 0.0f }, InSize, InTexture, InTilingFactor, InTintColour);
	}

	void CRenderer2D::DrawQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const TSharedPtr<CTexture>& InTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		Data->TextureShader->SetFloat4("u_Colour", InTintColour);
		Data->TextureShader->SetFloat("u_TilingFactor", InTilingFactor);
		Data->TextureShader->Bind();

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition) 
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });
		Data->TextureShader->SetMat4("u_Transform", Transform);

		InTexture->Bind();

		Data->QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(Data->QuadVertexArray);
	}

	void CRenderer2D::DrawRotatedQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const float InRotation, const glm::vec4 InColour)
	{
		Data->TextureShader->SetFloat4("u_Colour", InColour);
		Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		Data->WhiteTexture->Bind();

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition) 
			* glm::rotate(glm::mat4(1.0f), InRotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });
		Data->TextureShader->SetMat4("u_Transform", Transform);

		Data->QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(Data->QuadVertexArray);
	}

	void CRenderer2D::DrawRotatedQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const float InRotation, const TSharedPtr<CTexture>& InTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		Data->TextureShader->SetFloat4("u_Colour", InTintColour);
		Data->TextureShader->SetFloat("u_TilingFactor", InTilingFactor);
		Data->TextureShader->Bind();

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition)
			* glm::rotate(glm::mat4(1.0f), InRotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });
		Data->TextureShader->SetMat4("u_Transform", Transform);

		InTexture->Bind();

		Data->QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(Data->QuadVertexArray);
	}
}
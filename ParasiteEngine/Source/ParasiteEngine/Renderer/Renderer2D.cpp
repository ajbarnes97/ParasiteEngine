#include "pepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/ext/matrix_transform.hpp"


namespace Parasite
{
	struct SQuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct SRenderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		TSharedPtr<CVertexArray> QuadVertexArray;
		TSharedPtr<CVertexBuffer> QuadVertexBuffer;
		TSharedPtr<CShader> TextureShader;
		TSharedPtr<CTexture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		SQuadVertex* QuadVertexBufferBase = nullptr;
		SQuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<TSharedPtr<CTexture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // Slot 0 = white texture

		glm::vec4 QuadVertexPositions[4];
	};
	static SRenderer2DData Data;


	void CRenderer2D::Init()
	{
		Data.QuadVertexArray = CVertexArray::Create();

		Data.QuadVertexBuffer = CVertexBuffer::Create(Data.MaxVertices  * sizeof(SQuadVertex));
		Data.QuadVertexBuffer->SetLayout(
			{
				{ EShaderDataType::Float3,	"a_Position" },
				{ EShaderDataType::Float4,	"a_Colour" },
				{ EShaderDataType::Float2,	"a_TexCoord" },
				{ EShaderDataType::Float,	"a_TexIndex" },
				{ EShaderDataType::Float,	"a_TilingFactor" },
			}
		);
		Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);

		Data.QuadVertexBufferBase = new SQuadVertex[Data.MaxVertices];

		uint32_t* QuadIndices = new uint32_t[Data.MaxIndices];

		uint32_t Offset = 0;
		for (uint32_t Index = 0; Index < Data.MaxIndices; Index += 6)
		{
			QuadIndices[Index + 0] = Offset + 0;
			QuadIndices[Index + 1] = Offset + 1;
			QuadIndices[Index + 2] = Offset + 2;

			QuadIndices[Index + 3] = Offset + 2;
			QuadIndices[Index + 4] = Offset + 3;
			QuadIndices[Index + 5] = Offset + 0;

			Offset += 4;
		}

		TSharedPtr<CIndexBuffer> QuadIndexBuffer = CIndexBuffer::Create(QuadIndices, Data.MaxIndices);
		Data.QuadVertexArray->SetIndexBuffer(QuadIndexBuffer);
		delete[] QuadIndices;

		Data.WhiteTexture = CTexture2D::Create(1, 1);
		uint32_t TextureData = 0xffffffff;
		Data.WhiteTexture->SetData(&TextureData, sizeof(TextureData));

		int32_t Samplers[Data.MaxTextureSlots];
		for (uint32_t Index = 0; Index < Data.MaxTextureSlots; Index++)
		{
			Samplers[Index] = Index;
		}

		Data.TextureShader = CShader::Create("Assets/Shaders/Texture.glsl");
		Data.TextureShader->Bind();
		Data.TextureShader->SetIntArray("u_Textures", Samplers, Data.MaxTextureSlots);

		Data.TextureSlots[0] = Data.WhiteTexture;

		Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void CRenderer2D::Shutdown()
	{
	}

	void CRenderer2D::BeginScene(const CCamera& InCamera) 
	{
		Data.TextureShader->Bind();
		Data.TextureShader->SetMat4("u_ViewProjection", InCamera.GetViewProjectionMatrix());

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;
		Data.TextureSlotIndex = 1;
	}

	void CRenderer2D::BeginScene(const CCamera& InCamera, const glm::mat4& InTransform)
	{
		glm::mat4 ViewProj = InCamera.GetProjectionMatrix() * glm::inverse(InTransform);

		Data.TextureShader->Bind();
		Data.TextureShader->SetMat4("u_ViewProjection", ViewProj);

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;
		Data.TextureSlotIndex = 1;
	}

	void CRenderer2D::EndScene()
	{
		uint32_t DataSize = static_cast<uint32_t>(Data.QuadVertexBufferPtr - Data.QuadVertexBufferBase) * sizeof(SQuadVertex);
		Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferBase, DataSize);
		Flush();
	}

	void CRenderer2D::Flush()
	{
		for (uint32_t Index = 0; Index < Data.TextureSlotIndex; Index++)
		{
			Data.TextureSlots[Index]->Bind(Index);
		}

		CRenderCommand::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);
	}

	void CRenderer2D::DrawQuad(const glm::vec2& InPosition, const glm::vec2& InSize, const glm::vec4 InColour)
	{
		DrawQuad({ InPosition.x, InPosition.y, 0.0f }, InSize, InColour);
	}

	void CRenderer2D::DrawQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const glm::vec4 InColour)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition)
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });

		DrawQuad(Transform, InColour);
	}

	void CRenderer2D::DrawQuad(const glm::vec2& InPosition, const glm::vec2& InSize, const TSharedPtr<CTexture>& InTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		DrawQuad({ InPosition.x, InPosition.y, 0.0f }, InSize, InTexture, InTilingFactor, InTintColour);
	}

	void CRenderer2D::DrawQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const TSharedPtr<CTexture>& InTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition)
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });

		DrawQuad(Transform, InTexture, InTintColour, InTilingFactor);
	}

	void CRenderer2D::DrawQuad(const glm::vec2& InPosition, const glm::vec2& InSize, const TSharedPtr<CSubTexture2D>& InSubTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		DrawQuad({ InPosition.x, InPosition.y, 0.0f }, InSize, InSubTexture, InTilingFactor, InTintColour);
	}

	void CRenderer2D::DrawQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const TSharedPtr<CSubTexture2D>& InSubTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		constexpr size_t QuadVertexCount = 4;
		constexpr glm::vec4 Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* TextureCoords = InSubTexture->GetTexCoords();
		const TSharedPtr<CTexture2D> Texture = InSubTexture->GetTexture();

		float TextureIndex = 0.0f;
		for (uint32_t Index = 1; Index < Data.TextureSlotIndex; Index++)
		{
			if (*Data.TextureSlots[Index].get() == *Texture.get())
			{
				TextureIndex = static_cast<float>(Index);
				break;
			}
		}

		if (TextureIndex == 0.0f)
		{
			TextureIndex = static_cast<float>(Data.TextureSlotIndex);
			Data.TextureSlots[Data.TextureSlotIndex] = Texture;
			Data.TextureSlotIndex++;
		}

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition)
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });

		for (size_t Index = 0; Index < QuadVertexCount; Index++)
		{
			Data.QuadVertexBufferPtr->Position = Transform * Data.QuadVertexPositions[Index];
			Data.QuadVertexBufferPtr->Colour = InTintColour;
			Data.QuadVertexBufferPtr->TexCoord = TextureCoords[Index];
			Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
			Data.QuadVertexBufferPtr->TilingFactor = InTilingFactor;
			Data.QuadVertexBufferPtr++;
		}

		Data.QuadIndexCount += 6;
	}

	void CRenderer2D::DrawQuad(const glm::mat4& InTransform, const glm::vec4& InColour, const float InTilingFactor)
	{
		if (Data.QuadIndexCount >= SRenderer2DData::MaxIndices)
		{
			FlushAndReset();
		}

		constexpr size_t QuadVertexCount = 4;
		float TextureIndex = 0.0f; // White texture
		constexpr glm::vec2 TextureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		for (size_t Index = 0; Index < QuadVertexCount; Index++)
		{
			Data.QuadVertexBufferPtr->Position = InTransform * Data.QuadVertexPositions[Index];
			Data.QuadVertexBufferPtr->Colour = InColour;
			Data.QuadVertexBufferPtr->TexCoord = TextureCoords[Index];
			Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
			Data.QuadVertexBufferPtr->TilingFactor = InTilingFactor;
			Data.QuadVertexBufferPtr++;
		}

		Data.QuadIndexCount += 6;
	}

	void CRenderer2D::DrawQuad(const glm::mat4& InTransform, const TSharedPtr<CTexture>& InTexture, const glm::vec4& InTintColour, const float InTilingFactor)
	{
		if (Data.QuadIndexCount >= SRenderer2DData::MaxIndices)
		{
			FlushAndReset();
		}

		constexpr size_t QuadVertexCount = 4;
		float TextureIndex = 0.0f;
		constexpr glm::vec2 TextureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		for (uint32_t Index = 1; Index < Data.TextureSlotIndex; Index++)
		{
			if (*Data.TextureSlots[Index].get() == *InTexture.get())
			{
				TextureIndex = static_cast<float>(Index);
				break;
			}
		}

		if (TextureIndex == 0.0f)
		{
			TextureIndex = static_cast<float>(Data.TextureSlotIndex);
			Data.TextureSlots[Data.TextureSlotIndex] = InTexture;
			Data.TextureSlotIndex++;
		}

		for (size_t Index = 0; Index < QuadVertexCount; Index++)
		{
			Data.QuadVertexBufferPtr->Position = InTransform * Data.QuadVertexPositions[Index];
			Data.QuadVertexBufferPtr->Colour = InTintColour;
			Data.QuadVertexBufferPtr->TexCoord = TextureCoords[Index];
			Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
			Data.QuadVertexBufferPtr->TilingFactor = InTilingFactor;
			Data.QuadVertexBufferPtr++;
		}

		Data.QuadIndexCount += 6;
	}

	void CRenderer2D::DrawRotatedQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const float InRotation, const glm::vec4 InColour)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(InRotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });

		DrawQuad(Transform, InColour);
	}

	void CRenderer2D::DrawRotatedQuad(const glm::vec3& InPosition, const glm::vec2& InSize, const float InRotation, const TSharedPtr<CTexture>& InTexture, const float InTilingFactor, const glm::vec4& InTintColour)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), InPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(InRotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { InSize.x, InSize.y, 1.0f });

		DrawQuad(Transform, InTexture, InTintColour, InTilingFactor);
	}

	void CRenderer2D::FlushAndReset()
	{
		EndScene();

		Data.QuadIndexCount = 0;
		Data.QuadVertexBufferPtr = Data.QuadVertexBufferBase;
		Data.TextureSlotIndex = 1;
	}
}
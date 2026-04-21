#pragma once

#include "ParasiteEngine/Core/Core.h"
#include "string"


namespace Parasite
{
	class CTexture
	{
	public:
		virtual ~CTexture() = default;

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* InData, uint32_t InSize) = 0;

		virtual void Bind(uint32_t InSlot = 0) const = 0;

		virtual bool operator==(const CTexture& InOther) const = 0;
	};
	

	class CTexture2D : public CTexture
	{
	public:
		static TSharedPtr<CTexture2D> Create(const std::string& InFilePath);
		static TSharedPtr<CTexture2D> Create(uint32_t InWidth, uint32_t InHeight);
	};
}
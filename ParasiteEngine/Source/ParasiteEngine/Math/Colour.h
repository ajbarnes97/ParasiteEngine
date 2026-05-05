#pragma once

#include "glm/ext/vector_float4.hpp"


namespace Parasite
{
	class SColour
	{
	public:
		struct
		{
			float R, G, B, A;
		};

		SColour() : R(1.0f), G(1.0f), B(1.0f), A(1.0f) {}
		SColour(float InRed, float InGreen, float InBlue, float InAlpha)
			: R(InRed), G(InGreen), B(InBlue), A(InAlpha) {}
		SColour(float InRed, float InGreen, float InBlue)
			: R(InRed), G(InGreen), B(InBlue), A(1.0f) {
		}
		SColour(const glm::vec4& InColour)
			: R(InColour.r), G(InColour.g), B(InColour.b), A(InColour.a) { }

		operator glm::vec4() const { return { R, G, B, A }; }
		glm::vec4 ToVec4() const { return glm::vec4(R, G, B, A); }

		static inline SColour Red() { return SColour(1.0f, 0.0f, 0.0f, 1.0f); }
		static inline SColour Green() { return SColour(0.0f, 1.0f, 0.0f, 1.0f); }
		static inline SColour Blue() { return SColour(0.0f, 0.0f, 1.0f, 1.0f); }
		static inline SColour White() { return SColour(1.0f, 1.0f, 1.0f, 1.0f); }
	};
}
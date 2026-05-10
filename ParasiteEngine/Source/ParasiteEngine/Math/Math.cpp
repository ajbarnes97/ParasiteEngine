#include "pepch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"


namespace Parasite { namespace Math
{
	bool DecomposeTransform(const glm::mat4& InTransform, glm::vec3& OutTranslation, glm::vec3& OutRotation, glm::vec3& OutScale)
	{
		using namespace glm;

		mat4 LocalMatrix(InTransform);

		// Normalize the matrix
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<float>()))
		{
			return false;
		}

		// Isolate perspective
		if (epsilonNotEqual(LocalMatrix[0][3], static_cast<float>(0), epsilon<float>()) || 
			epsilonNotEqual(LocalMatrix[1][3], static_cast<float>(0), epsilon<float>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<float>(0), epsilon<float>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<float>(0);
			LocalMatrix[3][3] = static_cast<float>(1);
		}

		OutTranslation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		for (length_t I = 0; I < 3; ++I)
		{
			for (length_t J = 0; J < 3; ++J)
			{
				Row[I][J] = LocalMatrix[I][J];
			}
		}

		// Compute X scale factor and normalize first row
		OutScale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<float>(1));
		OutScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<float>(1));
		OutScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<float>(1));

		OutRotation.y = asin(-Row[0][2]);
		if (cos(OutRotation.y) != 0)
		{
			OutRotation.x = atan2(Row[1][2], Row[2][2]);
			OutRotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else
		{
			OutRotation.x = atan2(-Row[2][0], Row[1][1]);
			OutRotation.z = 0;
		}
		return true;
	}
}}
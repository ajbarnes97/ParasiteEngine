#pragma once
#include "glm/glm.hpp"

#define INDEX_NONE (-1)

namespace Parasite { namespace Math 
{
	bool DecomposeTransform(const glm::mat4& InTransform, glm::vec3& OutTranslation, glm::vec3& OutRotation, glm::vec3& OutScale);
}}
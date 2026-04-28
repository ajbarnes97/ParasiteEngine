#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "ParasiteEngine/Renderer/Camera.h"


namespace Parasite
{
	struct STagComponent
	{
	public:
		STagComponent() = default;
		STagComponent(const STagComponent&) = default;
		STagComponent(const std::string& InTag)
			: Tag(InTag) {
		}

		std::string Tag;
	};


	struct STransformComponent
	{
	public:
		STransformComponent() = default;
		STransformComponent(const STransformComponent&) = default;
		STransformComponent(const glm::mat4& InTransform)
			: Transform(InTransform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

	public:
		glm::mat4 Transform = glm::mat4(1.0f);
	};

	
	struct SSpriteRendererComponent
	{
	public:
		SSpriteRendererComponent() = default;
		SSpriteRendererComponent(const SSpriteRendererComponent&) = default;
		SSpriteRendererComponent(const glm::vec4& InColour)
			: Colour(InColour) {
		}

	public:
		glm::vec4 Colour;
	};

	struct SCameraComponent
	{
	public:
		SCameraComponent() = default;
		SCameraComponent(const SCameraComponent&) = default;
		SCameraComponent(const glm::mat4& InProjection)
			: Camera(InProjection) {
		}

		CCamera Camera; 
		bool bPrimaryCamera = true; // todo: Should be handled by scene
	};
}
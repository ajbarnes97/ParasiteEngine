#pragma once
#include "Scene.h"


namespace Parasite
{
	class CSceneSerializer
	{
	public:
		CSceneSerializer(const TSharedPtr<CScene>& InScene);

		void Serialize(const std::string& InFilepath);
		void SerializeBinary(const std::string& InFilepath);

		void Deserialize(const std::string& InFilepath);
		void DeserializeBinary(const std::string& InFilepath);

	private:
		TSharedPtr<CScene> Scene;
	};
}
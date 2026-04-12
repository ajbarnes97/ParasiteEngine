#pragma once

namespace Parasite
{
	class CTimestep
	{
	public:
		CTimestep(float InTime = 0.0f)
			: Time(InTime)
		{
		}

		inline operator float() const { return Time; }

		inline float GetSeconds() const { return Time; }
		inline float GetMilliseconds() const { return Time * 1000.0f; }
	private:
		float Time = 0.0f;
	};
}
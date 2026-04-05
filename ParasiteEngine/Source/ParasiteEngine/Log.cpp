#include "pepch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


namespace Parasite
{
	std::shared_ptr<spdlog::logger> CLog::s_CoreLogger;
	std::shared_ptr<spdlog::logger> CLog::s_ClientLogger;

	void CLog::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("PARASITE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
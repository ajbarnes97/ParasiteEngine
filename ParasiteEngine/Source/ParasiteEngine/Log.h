#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Parasite
{
	class PARASITE_API CLog
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log marcos
#define PE_CORE_FATAL(...)		::Parasite::CLog::GetCoreLogger()->critical(__VA_ARGS__)
#define PE_CORE_ERROR(...)		::Parasite::CLog::GetCoreLogger()->error(__VA_ARGS__)
#define PE_CORE_WARN(...)		::Parasite::CLog::GetCoreLogger()->warn(__VA_ARGS__)
#define PE_CORE_INFO(...)		::Parasite::CLog::GetCoreLogger()->info(__VA_ARGS__)
#define PE_CORE_LOG(...)		::Parasite::CLog::GetCoreLogger()->trace(__VA_ARGS__)
										   
#define PE_FATAL(...)			::Parasite::CLog::GetClientLogger()->critical(__VA_ARGS__)
#define PE_ERROR(...)			::Parasite::CLog::GetClientLogger()->error(__VA_ARGS__)
#define PE_WARN(...)			::Parasite::CLog::GetClientLogger()->warn(__VA_ARGS__)
#define PE_INFO(...)			::Parasite::CLog::GetClientLogger()->info(__VA_ARGS__)
#define PE_LOG(...)				::Parasite::CLog::GetClientLogger()->trace(__VA_ARGS__)
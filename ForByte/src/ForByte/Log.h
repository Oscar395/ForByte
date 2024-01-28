#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ForByte {
	class FORBYTE_API Log
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

// Core log macros
#define FB_CORE_INFO(...)   ::ForByte::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FB_CORE_TRACE(...)  ::ForByte::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FB_CORE_WARM(...)   ::ForByte::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FB_CORE_ERROR(...)  ::ForByte::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FB_CORE_FATAL(...)  ::ForByte::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define FB_INFO(...)        ::ForByte::Log::GetClientLogger()->info(__VA_ARGS__)
#define FB_TRACE(...)       ::ForByte::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FB_WARM(...)        ::ForByte::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FB_ERROR(...)       ::ForByte::Log::GetClientLogger()->error(__VA_ARGS__)
#define FB_FATAL(...)       ::ForByte::Log::GetClientLogger()->critical(__VA_ARGS__)


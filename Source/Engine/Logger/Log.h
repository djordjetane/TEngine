#pragma once

#include "spdlog/spdlog.h"

/*
	Class Log generates two loggers,
	one for Core(Engine) logging
	and one for logging from Engine Client(Game)
*/

namespace Engine{

	class Log
	{
	public:

		Log(const Log&) = delete;
		void operator=(const Log&) = delete;
		
		static void Init();
		inline static spdlog::logger& GetCoreLogger() { return *ms_CoreLogger; }
		inline static spdlog::logger& GetClientLogger() { return *ms_ClientLogger; }

	private:
		inline static std::shared_ptr<spdlog::logger> ms_CoreLogger;
		inline static std::shared_ptr<spdlog::logger> ms_ClientLogger;
	};

} // Engine


// Logs from Engine
#define LOG_CORE_TRACE(...) ::Engine::Log::GetCoreLogger().trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)  ::Engine::Log::GetCoreLogger().info(__VA_ARGS__)
#define LOG_CORE_WARN(...)  ::Engine::Log::GetCoreLogger().warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::Engine::Log::GetCoreLogger().error(__VA_ARGS__)
#define LOG_CORE_FATAL(...) ::Engine::Log::GetCoreLogger().critical(__VA_ARGS__)

// Logs from Client
#define LOG_TRACE(...) ::Engine::Log::GetClientLogger().trace(__VA_ARGS__)
#define LOG_INFO(...)  ::Engine::Log::GetClientLogger().info(__VA_ARGS__)
#define LOG_WARN(...)  ::Engine::Log::GetClientLogger().warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Engine::Log::GetClientLogger().error(__VA_ARGS__)
#define LOG_FATAL(...) ::Engine::Log::GetClientLogger().critical(__VA_ARGS__)

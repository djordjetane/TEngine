#include <precomp.h>
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


void Engine::Log::Init()
{	
	spdlog::set_pattern("%^[%T] %n: %v%$");

	ms_CoreLogger = std::move(spdlog::stdout_color_mt("ENGINE"));
	ms_CoreLogger->set_level(spdlog::level::trace);	

	ms_ClientLogger = std::move(spdlog::stdout_color_mt("GAME"));
	ms_ClientLogger->set_level(spdlog::level::trace);
}

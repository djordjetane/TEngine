#pragma once

/** @class Log
 * Singleton logger with static access
 * Wrapper for spdlog
 *
 * @see https://github.com/gabime/spdlog
 */
class Log
{
public:
    Log(const Log&) = delete;
    void operator=(const Log&) = delete;

    static void Init();

    inline static LoggerAPI& GetLogger() { return *ms_Logger; }

private:
    inline static TSharedPtr<spdlog::logger> ms_Logger = nullptr;
};

#define LOG_TRACE(...) Log::GetLogger().trace(__VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger().info(__VA_ARGS__)
#define LOG_WARN(...) Log::GetLogger().warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::GetLogger().error(__VA_ARGS__)
#define LOG_FATAL(...) Log::GetLogger().critical(__VA_ARGS__)
/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>
namespace BloxxEngine
{
class Log
{
    public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_CoreLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}

/// Logging macros
#define BE_CORE_TRACE(...) ::BloxxEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BE_CORE_INFO(...) ::BloxxEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BE_CORE_WARN(...) ::BloxxEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BE_CORE_ERROR(...) ::BloxxEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BE_CORE_FATAL(...) ::BloxxEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define BE_CLIENT_TRACE(...) ::BloxxEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BE_CLIENT_INFO(...) ::BloxxEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define BE_CLIENT_WARN(...) ::BloxxEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BE_CLIENT_ERROR(...) ::BloxxEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define BE_CLIENT_FATAL(...) ::BloxxEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
#pragma once

#include <iostream>
#include <sstream>
#include <string>

enum class LogCategory {
    Logger,
    Game,
    Engine,
    Texture,
    Shader,
    Model,
};

enum class LogLevel { Info, Warning, Error, Disabled };

class Logger {
   public:
    template <typename... Args>
    static void Log(LogCategory category, LogLevel level = LogLevel::Info,
                    Args&&... args) {
        std::ostringstream oss;
        (oss << ... << args);  // C++17 fold expression
        Log(oss.str(), category, level);
    }

    static void Log(const std::string& msg,
                    LogCategory category = LogCategory::Logger,
                    LogLevel level = LogLevel::Info);

    static void Shader(const std::string& msg, LogLevel level = LogLevel::Info);
    static void Texture(const std::string& msg,
                        LogLevel level = LogLevel::Info);
    static void Game(const std::string& msg, LogLevel level = LogLevel::Info);
    static void Engine(const std::string& msg, LogLevel level = LogLevel::Info);
    static void Model(const std::string& msg, LogLevel level = LogLevel::Info);

    // -- Enable / Disable Logging
    static void SetMinLevel(LogLevel level) { s_MinLogLevel = level; }
    static LogLevel GetMinLevel() { return s_MinLogLevel; }

   private:
    static inline LogLevel s_MinLogLevel =
        LogLevel::Info;  // -- Inline, define in header but static aswell, so
                         // only one occurence.
    Logger();
    static const char* CategoryToString(LogCategory category);
    static const char* CategoryToColor(LogCategory category);
    static const char* LevelToString(LogLevel level);
    static const char* LevelToColor(LogLevel level);
};

#define LOG(...) Logger::Log(LogCategory::Engine, LogLevel::Info, __VA_ARGS__)
#define LOG_INFO(cat, ...) Logger::Log(cat, LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(cat, ...) Logger::Log(cat, LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(cat, ...) Logger::Log(cat, LogLevel::Error, __VA_ARGS__)

#include "Logger.h"

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"

void Logger::Log(const std::string& msg, LogCategory category, LogLevel level) {
    if (level < s_MinLogLevel) return;

    std::cout << BOLD << LevelToColor(level) << "[" << LevelToString(level)
              << "] " << CategoryToColor(category) << "["
              << CategoryToString(category) << "] " << RESET << msg << RESET
              << std::endl;
}

void Logger::Shader(const std::string& msg, LogLevel level) {
    Log(msg, LogCategory::Shader, level);
}
void Logger::Texture(const std::string& msg, LogLevel level) {
    Log(msg, LogCategory::Texture, level);
}
void Logger::Game(const std::string& msg, LogLevel level) {
    Log(msg, LogCategory::Game, level);
}
void Logger::Engine(const std::string& msg, LogLevel level) {
    Log(msg, LogCategory::Engine, level);
}
void Logger::Model(const std::string& msg, LogLevel level) {
    Log(msg, LogCategory::Model, level);
}

const char* Logger::CategoryToString(LogCategory category) {
    switch (category) {
        case LogCategory::Logger:
            return "LOGGER";
        case LogCategory::Game:
            return "GAME";
        case LogCategory::Engine:
            return "ENGINE";
        case LogCategory::Texture:
            return "TEXTURE";
        case LogCategory::Shader:
            return "SHADER";
        case LogCategory::Model:
            return "MODEL";
        default:
            return "UNKNOWN";
    }
}

const char* Logger::CategoryToColor(LogCategory category) {
    switch (category) {
        case LogCategory::Logger:
            return GREEN;
        case LogCategory::Game:
            return BLUE;
        case LogCategory::Engine:
            return CYAN;
        case LogCategory::Texture:
        case LogCategory::Model:
            return YELLOW;
        case LogCategory::Shader:
            return MAGENTA;
        default:
            return RESET;
    }
}

const char* Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

const char* Logger::LevelToColor(LogLevel level) {
    switch (level) {
        case LogLevel::Info:
            return GREEN;
        case LogLevel::Warning:
            return YELLOW;
        case LogLevel::Error:
            return RED;
        default:
            return RESET;
    }
}

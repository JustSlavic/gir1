#include <logging/logging.h>
#include <iostream>
#include <ctime>
#include <iomanip>


LogLocalContext::LogLocalContext() noexcept : name(nullptr) {}

LogLocalContext::LogLocalContext(const char *name) noexcept : name(name) {}

Log::Log(LogLocalContext ctx) : context(ctx) {}

Log::~Log() {
    LogGlobalContext::instance().write(log, level, context);
}

Log &Log::error() {
    this->level = Level::Error;
    return *this;
}

Log &Log::warning() {
    this->level = Level::Warning;
    return *this;
}

Log &Log::info() {
    this->level = Level::Info;
    return *this;
}

Log &Log::debug() {
    this->level = Level::Debug;
    return *this;
}

const char *log_level_to_cstr(Log::Level level) {
    switch (level) {
    case Log::Level::Error: return "ERROR";
    case Log::Level::Warning: return "WARNING";
    case Log::Level::Info: return "INFO";
    case Log::Level::Debug: return "DEBUG";
        default: return "";
    }
}

LogGlobalContext &LogGlobalContext::instance() {
    static LogGlobalContext instance;
    return instance;
}

void LogGlobalContext::write(std::stringstream &log, Log::Level log_level, LogLocalContext ctx) {
    if (log_level < level) return;

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::cout << std::put_time(&tm, "%F %T ")
              << log_level_to_cstr(log_level);

    if (ctx.name) {
        std::cout << " [" << ctx.name << "] " << log.str() << std::endl;
    } else {
        std::cout << " " << log.str() << std::endl;
    }
}

LogGlobalContext &LogGlobalContext::set_level(Log::Level new_level) {
    this->level = new_level;
    return *this;
}

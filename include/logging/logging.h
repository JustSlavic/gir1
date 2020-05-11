#ifndef GIR1_LOGGING_H
#define GIR1_LOGGING_H

#include <sstream>
#include <ostream>


#define LOG_DEBUG Log(log_ctx_).debug()
#define LOG_INFO Log(log_ctx_).info()
#define LOG_WARNING Log(log_ctx_).warning()
#define LOG_ERROR Log(log_ctx_).error()


struct LogLocalContext {
    const char *name;

    LogLocalContext() noexcept;
    explicit LogLocalContext(const char *name) noexcept;
};

struct Log {
    enum class Level {
        Debug,
        Info,
        Warning,
        Error,
        Disabled,
    };

    LogLocalContext context;
    Level level = Level::Disabled;
    std::stringstream log;

    explicit Log(LogLocalContext ctx = LogLocalContext());
    ~Log();

    Log &error();
    Log &warning();
    Log &info();
    Log &debug();
};

template <typename T>
Log &operator<<(Log &logger, T&& data) {
    logger.log << data;
    return logger;
}

struct LogGlobalContext {
    Log::Level level = Log::Level::Debug;

    static LogGlobalContext &instance();
    LogGlobalContext &set_level(Log::Level level);

    void write(std::stringstream &log, Log::Level log_level, LogLocalContext ctx);

private:
    LogGlobalContext() = default;
};

#endif //GIR1_LOGGING_H

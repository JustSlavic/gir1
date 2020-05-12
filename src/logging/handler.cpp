#include <logging/handler.h>
#include <sstream>


LogFileHandler::LogFileHandler(const char *filename)
    : output(filename, std::ios::app)
{
    if (output.bad()) throw std::runtime_error("Cannot open log file!");
}

void LogFileHandler::write(const std::stringstream &log) {
    output << log.str() << '\n';
    output.flush();
}

LogStreamHandler::LogStreamHandler(std::ostream &stream)
    : output(stream)
{
}

void LogStreamHandler::write(const std::stringstream &log) {
    output << log.str() << '\n';
    output.flush();
}

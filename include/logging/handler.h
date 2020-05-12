#ifndef GIR1_HANDLER_H
#define GIR1_HANDLER_H

#include <fstream>


struct LogHandler {
    virtual void write(const std::stringstream& log) = 0;
};

struct LogFileHandler : public LogHandler {
    std::ofstream output;

    explicit LogFileHandler(const char *filename);
    void write(const std::stringstream& log) override;
};

struct LogStreamHandler : public LogHandler {
    std::ostream &output;

    explicit LogStreamHandler(std::ostream &stream);
    void write(const std::stringstream& log) override;
};


#endif //GIR1_HANDLER_H

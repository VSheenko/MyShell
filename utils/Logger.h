#ifndef MYSHELL_LOGGER_H
#define MYSHELL_LOGGER_H

#include <string>
#include <fstream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    explicit Logger(std::string);
    ~Logger();

    void Log(LogLevel, const std::string&);
private:
    std::ofstream logFileStream;

    std::string GetLabel(LogLevel);
};


#endif //MYSHELL_LOGGER_H

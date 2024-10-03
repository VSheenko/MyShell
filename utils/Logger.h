#ifndef MYSHELL_LOGGER_H
#define MYSHELL_LOGGER_H

#include <string>
#include <fstream>

enum class LogLevel {
    DEBUG = 0,
    INFO,
    WARNING,
    ERR
};

class Logger {
public:
    explicit Logger(const std::string s_logs_path);
    ~Logger();

    void Log(LogLevel, const std::string&);
private:
    std::ofstream* logFileStream;

    std::string GetLabel(LogLevel);
};


#endif //MYSHELL_LOGGER_H

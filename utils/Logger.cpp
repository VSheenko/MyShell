#include "Logger.h"
#include <filesystem>
#include <ctime>
#include <iostream>

namespace fs = std::filesystem;

Logger::Logger(const std::string s_path) {
    fs::path log_file_path = s_path;
    fs::create_directories(log_file_path);

    time_t now_time = time(NULL);
    std::tm* local_time = localtime(&now_time);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    log_file_path.append(std::string(buffer));

    logFileStream = new std::ofstream (log_file_path.string());
}

Logger::~Logger() {
    logFileStream->close();
    delete logFileStream;
    logFileStream = nullptr;
}

void Logger::Log(LogLevel level, const std::string &msg) {
    time_t now_time = time(NULL);
    std::tm* local_time = localtime(&now_time);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", local_time);

    *logFileStream << "[ " << GetLabel(level) << " ] [ " << std::string(buffer) << " ] " <<  msg << std::endl;
}

std::string Logger::GetLabel(LogLevel logLevel) {
    switch (logLevel) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERR: return "ERROR";
        default: return "UNKNOWN";
    }
}

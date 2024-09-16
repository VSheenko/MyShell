#include "Logger.h"
#include <filesystem>
#include <ctime>
#include <iostream>
#include <format>

namespace fs = std::filesystem;

Logger::Logger(std::string s_path) {
    fs::path log_file_path = s_path;

    time_t now_time = time(NULL);
    std::tm* local_time = localtime(&now_time);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    log_file_path.append(std::string(buffer));
    std::cout << log_file_path.string() << '\n';
    logFileStream.open(log_file_path.string());
}

Logger::~Logger() {
    logFileStream.close();
}

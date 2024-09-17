#ifndef MYSHELL_SHELL_H
#define MYSHELL_SHELL_H

#include <string>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include "utils/Logger.h"
#include <regex>

namespace fs = std::filesystem;

class Shell {
private:
    std::string cur_user;
    std::string pc_name;
    fs::path cur_path;
    Logger* logger;

    void SetUserName(std::string);
    void SetPcName(std::string);
    void SetCurPath(std::string);

public:
    Shell(const std::string&, const std::string&, const std::string&);
    int ExecShell();
};


#endif //MYSHELL_SHELL_H

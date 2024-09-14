#ifndef MYSHELL_SHELL_H
#define MYSHELL_SHELL_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class Shell {
private:
    std::string cur_user;
    std::string pc_name;
    fs::path cur_path;
    fs::path log_path;

public:
    Shell();
    Shell(const std::string&);
};


#endif //MYSHELL_SHELL_H

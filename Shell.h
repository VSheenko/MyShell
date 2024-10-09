#ifndef MYSHELL_SHELL_H
#define MYSHELL_SHELL_H

#include <string>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include "utils/XmlConfigWorker.h"
#include "utils/Logger.h"
#include "utils/UtilsMini.h"
#include "struct/SharedData.h"
#include "utils/ArchiveZipWorker.h"
#include <regex>
#include <map>
#include <windows.h>

#define EXIT_COMMAND "exit"


namespace fs = std::filesystem;



class Shell {
private:
    std::string cur_user;
    std::string pc_name;

    fs::path archive_path;
    std::string cur_path_in_archive;

    std::map<std::string, fs::path> command_links;

    Logger* logger = nullptr;

    SharedData* sharedData;

    void SetUserName(const std::string&);
    void SetPcName(const std::string&);
    void SetArchivePath(const std::string&);
    void SetCurPath(std::string);

    void PrintSystemInvitation();
    void ExecCommand(const std::string& args);
    bool InitCommands(const std::string& commands_root_folder);

public:
    explicit Shell(const std::string& s_config_path);
    ~Shell();

    int ExecShell();
};





#endif //MYSHELL_SHELL_H

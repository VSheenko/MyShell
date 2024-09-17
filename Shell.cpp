#include "Shell.h"


Shell::Shell(const std::string& s_user, const std::string& s_pc_name, const std::string& s_archive_path) {
    SetUserName(s_user);
    SetPcName(s_pc_name);




}

void Shell::SetUserName(const std::string s_name) {
    const std::regex r("[a-z_][a-z0-9_-]");

    if (!std::regex_match(s_name, r) || s_name.size() > 32) {
        std::cerr << "Incorrect user name format << '\n";

        if (pc_name.empty()){
            std::cerr << "The name Ivan will be used" << '\n';
            pc_name = "Ivan";
        }
    } else {
        cur_user = s_name;
    }
}

void Shell::SetPcName(const std::string s_name) {
    const std::regex r("[a-z_][a-z0-9_-]");

    if(!std::regex_match(s_name, r) || s_name.size() > 32) {
        std::cerr << "Incorrect pc name format << '\n";

        if (pc_name.empty()){
            std::cerr << "The name MainPc will be used" << '\n';
            pc_name = "MainPc";
        }
    } else {
        pc_name = s_name;
    }
}

void Shell::SetCurPath(const std::string s_path) {

}

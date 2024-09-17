#ifndef MYSHELL_ARCHIVEZIPWORKER_H
#define MYSHELL_ARCHIVEZIPWORKER_H


#include <string>
#include "miniz.h"
#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

class ArchiveZipWorker {
public:
    ArchiveZipWorker(std::string);
    bool DeleteFile(std::string);
    bool DeleteDirectory(std::string);
    bool AddFile(std::string s_path);
    void ChangeDirectory(std::string);
    bool GetAllFilesNameInFolder(std::string s_path, std::vector<std::string>&);
    bool GetFileData(const std::string &, std::vector<unsigned char>&);
private:
    std::vector<std::string> Split(std::string, const std::string);
    fs::path cur_directory;
    fs::path archive_path;
};


#endif //MYSHELL_ARCHIVEZIPWORKER_H

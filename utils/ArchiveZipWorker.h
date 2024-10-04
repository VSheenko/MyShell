#ifndef MYSHELL_ARCHIVEZIPWORKER_H
#define MYSHELL_ARCHIVEZIPWORKER_H


#include <string>
#include "miniz.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include "UtilsMini.h"

namespace fs = std::filesystem;

class ArchiveZipWorker {
public:
    ArchiveZipWorker(const std::string&);
    bool DeleteFile(std::string);
    bool DeleteDirectory(std::string);
    bool FolderExist(std::string s_path);
    bool FileExist(std::string s_path);
    bool AddFile(std::string s_path);
    void ChangeDirectory(std::string);
    bool GetAllFilesNameInFolder(std::string s_path, std::vector<std::string>& res_buffer);
    bool GetFileData(std::string, std::vector<char>&);
    std::string GetAbsPathDir(std::string inp_path, std::string cur_path_in_archive);
    std::string GetAbsPathFile(std::string inp_path, std::string cur_path_in_archive);
    static std::string GetUnixPath(std::string);
    static std::string NormalizeVirtualPath(const std::string& temp_path);
private:
    bool ZipReaderInit(mz_zip_archive&);
    bool FindPath(const std::string& s_path);

    fs::path archive_path;
};


#endif //MYSHELL_ARCHIVEZIPWORKER_H

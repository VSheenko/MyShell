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
    explicit ArchiveZipWorker(const std::string&);
    bool FolderExist(std::string s_path);
    bool FileExist(std::string s_path);
    bool GetAllFilesNameInFolder(std::string s_path, std::vector<std::string>& res_buffer);
    bool GetFileData(std::string, std::vector<char>&);
    bool GetAbsPathDir(std::string& result_path, std::string inp_path, std::string cur_path_in_archive);
    bool GetAbsPathFile(std::string& result_path, std::string inp_path, std::string cur_path_in_archive);
    static std::string GetUnixPath(std::string);
    static std::string NormalizeVirtualPath(const std::string& temp_path);
private:
    void ZipReaderInit(mz_zip_archive&);
    bool FindPath(const std::string& s_path);

    fs::path archive_path;
};


#endif //MYSHELL_ARCHIVEZIPWORKER_H

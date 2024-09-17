#ifndef MYSHELL_ARCHIVEZIPWORKER_H
#define MYSHELL_ARCHIVEZIPWORKER_H


#include <string>
#include "miniz.h"
#include <filesystem>

namespace fs = std::filesystem;

class ArchiveZipWorker {
public:
    ArchiveZipWorker(std::string);
    bool DeleteFile(std::string);
    bool DeleteDirectory(std::string);
    bool AddFile();
private:
    mz_zip_archive archive;
    fs::path cur_directory;
};


#endif //MYSHELL_ARCHIVEZIPWORKER_H

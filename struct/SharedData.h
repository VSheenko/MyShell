#ifndef LS_SHAREDDATA_H
#define LS_SHAREDDATA_H

#include <windows.h>
#include <string>
#include <stdexcept>

#define SHARED_MEMORY_NAME "SharedMemoryForStaticField"
#define BUF_SIZE_FOR_SHARED_MEM 128

struct SharedData {
private:
    std::string file_mapping_name;

private:
    HANDLE hMapFile;
    char* pBufViewOfFile;

public:
    std::string s_archive_path;
    std::string s_path_in_archive;

    explicit SharedData(const std::string &fileMappingName = SHARED_MEMORY_NAME);

    virtual ~SharedData();

    void CreateFileMap();
    void OpenFileMap();

    void UpdateFields(const std::string &sArchivePath, const std::string &sPathInArchive);

    void SerializeSharedData();
    void DeserializeSharedData();
};

#endif //LS_SHAREDDATA_H

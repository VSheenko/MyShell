#include "SharedData.h"

void SharedData::CreateFileMap() {
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            BUF_SIZE_FOR_SHARED_MEM,
            file_mapping_name.c_str());

    if (hMapFile == NULL)
        std::runtime_error("Failed to create shared memory");

    pBufViewOfFile = (char*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            BUF_SIZE_FOR_SHARED_MEM);

    if (pBufViewOfFile == NULL) {
        std::runtime_error("Failed to map memory area");
        CloseHandle(hMapFile);
    }
}

void SharedData::SerializeSharedData() {
    char* cur_pos = pBufViewOfFile;

    auto writeString = [&](const std::string& str) {
        size_t length = str.size();

        memcpy(cur_pos, &length, sizeof(size_t));
        cur_pos += sizeof(size_t);

        memcpy(cur_pos, str.c_str(), length);
        cur_pos += length;
    };

    if (s_archive_path.size() + s_path_in_archive.size() > BUF_SIZE_FOR_SHARED_MEM)
        std::runtime_error("Recording larger than buffer");

    *pBufViewOfFile = '\0';

    writeString(s_archive_path);
    writeString(s_path_in_archive);
}

void SharedData::DeserializeSharedData() {
    const char* cur_pos = pBufViewOfFile;

    auto readString = [&](std::string& str) {
        size_t length;
        memcpy(&length, cur_pos, sizeof(size_t));
        cur_pos += sizeof(size_t);

        str = std::string(cur_pos, length);
        cur_pos += length;
    };

    std::string temp_str;

    readString(s_archive_path);
    readString(s_path_in_archive);
}

void SharedData::OpenFileMap() {
    hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            file_mapping_name.c_str());

    if (hMapFile == NULL)
        std::runtime_error("Failed to open shared memory");

    pBufViewOfFile = (char*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            BUF_SIZE_FOR_SHARED_MEM);

    if (pBufViewOfFile == NULL) {
        std::runtime_error("Failed to map memory area");
        CloseHandle(hMapFile);
    }
}

SharedData::~SharedData() {
    UnmapViewOfFile(pBufViewOfFile);
    CloseHandle(hMapFile);
}

void SharedData::UpdateFields(const std::string &sArchivePath, const std::string &sPathInArchive) {
    s_archive_path = sArchivePath,
    s_path_in_archive = sPathInArchive;
}

SharedData::SharedData(const std::string &fileMappingName) : file_mapping_name(fileMappingName) {}

#include <windows.h>
#include "../../utils/ArchiveZipWorker.h"

#define SHARED_MEMORY_NAME "SharedMemoryForStaticField"
#define BUF_SIZE_FOR_SHARED_MEM 64

std::string archive_path;
std::string cur_path_in_archive;

void DeserializeStaticFields(const char* buffer) {
    const char* cur_pos = buffer;

    auto readString = [&](std::string& str) {
        size_t length;
        memcpy(&length, cur_pos, sizeof(size_t));
        cur_pos += sizeof(size_t);

        str = std::string(cur_pos, length);
        cur_pos += length;
    };

    std::string temp_str;

    readString(temp_str);
    archive_path = temp_str;

    readString(temp_str);
    cur_path_in_archive = temp_str;
}

int main(int argc, char* argv[]) {
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHARED_MEMORY_NAME);
    char* pBuf = (char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE_FOR_SHARED_MEM);

    DeserializeStaticFields(pBuf);

    std::cout << archive_path << " <==> " << cur_path_in_archive  << '\n';
}
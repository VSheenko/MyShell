#include "../../utils/ArchiveZipWorker.h"
#include "../../struct/SharedData.h"
#include <string>

std::string GetPathInArchive() {
    SharedData sharedData;

    sharedData.OpenFileMap();
    sharedData.DeserializeSharedData();

    return sharedData.s_path_in_archive;
}

std::string GetArchivePath() {
    SharedData sharedData;

    sharedData.OpenFileMap();
    sharedData.DeserializeSharedData();

    return sharedData.s_archive_path;
}

void PrintContent(std::vector<std::string>& content) {
    for (int i = 0; i < content.size(); i++) {
        std::cout << content[i];
        if (i != content.size() - 1)
            std::cout << "  ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // argv[0] - Путь исполняемого файла
    // argv[1 - ...] - что нам нужно рассказать)
    ArchiveZipWorker zip_worker(GetArchivePath());
    const std::string s_path_in_archive = GetPathInArchive();
    std::vector<std::string> res;



    if (argc == 1) {
        zip_worker.GetAllFilesNameInFolder(s_path_in_archive, res);

        if (res.empty())
            return 0;

        PrintContent(res);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        std::string temp_path = argv[i];
        temp_path = zip_worker.GetAbsPath(temp_path, s_path_in_archive);

        if (temp_path == "No such directory") {
            std::cerr << argv[i] << ": No such directory" << std::endl;
            return 1;
        }

        zip_worker.GetAllFilesNameInFolder(temp_path, res);

        if (argc > 2)
            std::cout << argv[i] << ":" << std::endl;

        PrintContent(res);

        if (i != argc - 1)
            std::cout << std::endl;
    }
}
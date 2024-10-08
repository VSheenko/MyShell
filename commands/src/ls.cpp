#include "../../utils/ArchiveZipWorker.h"
#include "../../struct/SharedData.h"
#include <string>

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

    SharedData sharedData;
    sharedData.OpenFileMap();
    sharedData.DeserializeSharedData();

    ArchiveZipWorker zip_worker(sharedData.s_archive_path);
    const std::string s_path_in_archive = sharedData.s_path_in_archive;
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
        std::string abs_path;


        if (!zip_worker.GetAbsPathDir(abs_path, temp_path, s_path_in_archive)) {
            std::cerr << argv[i] << ": No such directory" << std::endl;
            return 1;
        }

        zip_worker.GetAllFilesNameInFolder(abs_path, res);

        if (argc > 2)
            std::cout << argv[i] << ":" << std::endl;

        PrintContent(res);

        if (i != argc - 1)
            std::cout << std::endl;
    }
}
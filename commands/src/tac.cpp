#include "../../utils/ArchiveZipWorker.h"
#include "../../struct/SharedData.h"


void Print(std::vector<char>& buffer) {
    std::string s(buffer.begin(), buffer.end());
    std::vector<std::string> lines = utils::Split(s, "\n");

    for (int i = lines.size() - 1; i >= 0; i--) {
        std::cout << lines[i] << std::endl;
    }
}

int main(int argc, char* argv[]) {
    SharedData sharedData;

    sharedData.OpenFileMap();
    sharedData.DeserializeSharedData();

    ArchiveZipWorker zip_worker(sharedData.s_archive_path);
    std::string s_path_in_archive = sharedData.s_path_in_archive;

    //argv[1- ...] - что нам нужно рассказать)

    std::string abs_path;
    for (int i = 1; i < argc; i++) {
        if (!zip_worker.GetAbsPathFile(abs_path, argv[i], s_path_in_archive)) {
            std::cerr << "tac: " << argv[i] << ": No such file or directory" << std::endl;
            return 1;
        }

        std::vector<char> buffer;

        if (!zip_worker.GetFileData(abs_path, buffer)) {
            std::cerr << "tac: " << argv[i] << ": Error reading file" << std::endl;
            return 1;
        }

        Print(buffer);
    }

}
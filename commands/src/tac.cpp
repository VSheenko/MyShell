#include "../../utils/ArchiveZipWorker.h"
#include "../../utils/UtilsMini.h"
#include "../../struct/SharedData.h"
#include <string>

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

    for (int i = 1; i < argc; i++) {
        std::string abs_path = zip_worker.GetAbsPath(argv[i], s_path_in_archive);

        if (abs_path == "No such file or directory") {
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
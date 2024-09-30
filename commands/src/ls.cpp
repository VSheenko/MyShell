#include "../../utils/ArchiveZipWorker.h"
#include "../../struct/SharedData.h"

int main(int argc, char* argv[]) {
    SharedData sharedData;

    sharedData.OpenFileMap();
    sharedData.DeserializeSharedData();

    std::cout << sharedData.s_archive_path << " <==> " << sharedData.s_path_in_archive  << '\n';
}
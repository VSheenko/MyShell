#include "Shell.h"

#define APPSETTINGS_FILE "./appsettings.xml"

int main(int argc, char* argv[]) {
    Shell shell(APPSETTINGS_FILE);

    if (shell.ExecShell()) {
        std::cout << "Error" << std::endl;
        std::cout << "Press any key to exit" << std::endl;
        std::cin.get();
        return 1;
    }
}

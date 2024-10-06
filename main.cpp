#include "Shell.h"

int main() {
    Shell shell("Z:\\Projects\\MyShell\\cmake-build-debug\\appsettings.xml");
    if (!shell.ExecShell()) {
        std::cout << "Error" << std::endl;
        std::cout << "Press any key to exit" << std::endl;
        std::cin.get();
        return 1;
    }
}

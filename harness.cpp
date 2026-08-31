#include <windows.h>

#include <iostream>

#include "exception.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: .\\" << argv[0] << " <library_dir> <library_name>" << std::endl;
    }

    SetCurrentDirectoryA(argv[1]);
    LoadLibraryA(argv[2]);
    RaiseException(MODULE_LOADED, 0, 0, nullptr);

    return EXIT_SUCCESS;
}

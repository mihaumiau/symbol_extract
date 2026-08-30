#include <windows.h>
#include <shlwapi.h>

#include <iostream>

#include "exception.hpp"

CHAR libraryPath[MAX_PATH] = {0};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: .\\" << argv[0] << " <library_dir> <library_name>" << std::endl;
    }

    SetCurrentDirectoryA(argv[1]);
    PathAppendA(libraryPath, argv[1]);
    PathAppendA(libraryPath, argv[2]);
    LoadLibraryA(libraryPath);
    RaiseException(MODULE_LOADED, 0, 0, nullptr);

    return EXIT_SUCCESS;
}
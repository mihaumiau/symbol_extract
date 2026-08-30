#include <windows.h>

#include <iostream>

#include "exception.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: ./harness.exe <library_path>" << std::endl;
    }

    LoadLibraryA(argv[1]);
    RaiseException(MODULE_LOADED, 0, 0, nullptr);
    Sleep(INFINITE);

    return EXIT_SUCCESS;
}
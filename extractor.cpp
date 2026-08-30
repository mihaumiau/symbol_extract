#include <windows.h>
#include <dbgeng.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "exception.hpp"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "usage: .\\" << argv[0] << " <library_dir> <library_name> <symbol_server> [<symbol_name>...] <disassemble_out>" << std::endl;

        return EXIT_FAILURE;
    }

    HRESULT result;

    IDebugClient* client = nullptr;

    if (FAILED(result = DebugCreate(__uuidof(IDebugClient), reinterpret_cast<void**>(&client)))) {
        std::cerr << "client->QueryInterface(__uuidof(IDebugClient), ...) failed: 0x" << std::hex << result << std::endl;  

        return EXIT_FAILURE;
    }

    IDebugControl* control = nullptr;

    if (FAILED(result = client->QueryInterface(__uuidof(IDebugControl), reinterpret_cast<void**>(&control)))) {
        std::cerr << "client->QueryInterface(__uuidof(IDebugControl), ...) failed: 0x" << std::hex << result << std::endl;  

        client->Release();

        return EXIT_FAILURE;
    }

    if (FAILED(result = client->CreateProcess(0, const_cast<char*>((std::string("harness.exe \"") + argv[1] + "\" \"" + argv[2] + "\"").c_str()), DEBUG_PROCESS))){
        std::cerr << "client->CreateProcess(...) failed: 0x" << std::hex << result << std::endl;  

        control->Release();
        client->Release();

        return EXIT_FAILURE;
    }

    while (true) {
        if (FAILED(result = control->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE))) {
            std::cerr << "control->WaitForEvent(...) failed: 0x" << std::hex << result << std::endl;

            client->EndSession(DEBUG_END_PASSIVE);

            control->Release();
            client->Release();

            return EXIT_FAILURE;
        }

        ULONG type = 0;
        ULONG processId = 0;
        ULONG threadId = 0;

        if (FAILED(control->GetLastEventInformation(&type, &processId, &threadId, nullptr, 0, nullptr, nullptr, 0, nullptr))) {
            std::cerr << "control->GetLastEventInformation(...) failed: 0x" << std::hex << result << std::endl;

            client->EndSession(DEBUG_END_PASSIVE);

            control->Release();
            client->Release();

            return EXIT_FAILURE;
        }

        if (type == DEBUG_EVENT_EXCEPTION) {
            DEBUG_LAST_EVENT_INFO_EXCEPTION exceptionInfo{};

            if (SUCCEEDED(control->GetLastEventInformation(&type, &processId, &threadId, &exceptionInfo, sizeof(exceptionInfo), nullptr, nullptr, 0, nullptr))) {
                if (exceptionInfo.ExceptionRecord.ExceptionCode == MODULE_LOADED) {
                    break;
                }
            } else {
                std::cerr << "control->GetLastEventInformation(...) failed: 0x" << std::hex << result << std::endl;

                client->EndSession(DEBUG_END_PASSIVE);

                control->Release();
                client->Release();

                return EXIT_FAILURE;
            }
        }

        control->SetExecutionStatus(DEBUG_STATUS_GO);
    }

    IDebugSymbols* symbols = nullptr;

    if (FAILED(client->QueryInterface(__uuidof(IDebugSymbols), reinterpret_cast<void**>(&symbols)))) {
        std::cerr << "client->QueryInterface(__uuidof(IDebugSymbols), ...) failed: 0x" << std::hex << result << std::endl;  

        control->Release();
        client->Release();

        return EXIT_FAILURE;
    }

    if (FAILED(result = symbols->SetSymbolPath(argv[3]))) {
        std::cerr << "symbols->SetSymbolPath(...) failed: 0x" << std::hex << result << std::endl;

        symbols->Release();
        control->Release();
        client->Release();

        return EXIT_FAILURE;
    }


    if (FAILED(result = symbols->Reload(argv[2]))) {
        std::cerr << "symbols->Reload(...) failed: 0x" << std::hex << result << std::endl;

        client->EndSession(DEBUG_END_PASSIVE);


        symbols->Release();
        control->Release();
        client->Release();

        return EXIT_FAILURE;
    }

    ULONG64 offset = 0;

    for (int i = 4; i < argc - 1; i++) {
        std::stringstream ss;

        if (FAILED(result = symbols->GetOffsetByName(argv[i], &offset))) {
            std::cerr << "symbols->GetOffsetByName(...) failed: 0x" << std::hex << result << std::endl;

            client->EndSession(DEBUG_END_PASSIVE);

            symbols->Release();
            control->Release();
            client->Release();

            return EXIT_FAILURE;
        }

        CHAR symbolName[512];
        ULONG symbolNameSize;
        ULONG64 displacement;

        CHAR line[512];
        ULONG lineSize;

        while (true) {
            if (FAILED(result = symbols->GetNameByOffset(offset, symbolName, sizeof(symbolName), &symbolNameSize, &displacement))) {
                std::cerr << "control->GetNameByOffset(...) failed: 0x" << std::hex << result << std::endl;

                client->EndSession(DEBUG_END_PASSIVE);

                symbols->Release();
                control->Release();
                client->Release();

                return EXIT_FAILURE;
            }

            if (strcmp(symbolName, argv[i]) != 0) {
                break;
            }

            if (FAILED(control->Disassemble(offset, DEBUG_DISASM_EFFECTIVE_ADDRESS, line, sizeof(line), &lineSize, &offset))) {
                std::cerr << "control->Disassemble(...) failed: 0x" << std::hex << result << std::endl;

                client->EndSession(DEBUG_END_PASSIVE);

                symbols->Release();
                control->Release();
                client->Release();

                return EXIT_FAILURE;
            }

            ss << line;
            std::cout << line;
        }

        std::string fileName = std::string(argv[argc - 1]) + "/" + argv[i] + ".txt";
        std::ofstream file(fileName);

        if (!file) {
            std::cerr << "failed to open output file: " << fileName << std::endl;

            client->EndSession(DEBUG_END_PASSIVE);

            symbols->Release();
            control->Release();
            client->Release();

            return EXIT_FAILURE;
        }

        file << ss.str();
    }

    return EXIT_SUCCESS;
}
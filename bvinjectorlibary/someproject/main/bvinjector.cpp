#include "pch.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include "bvinjector.h"
#include "injector.h"

namespace fs = std::filesystem;

namespace bvinjector {

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

DWORD GetProcessIdByName(const std::string& name) {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32 entry = { 0 };
    entry.dwSize = sizeof(entry);
    if (Process32First(snapshot, &entry)) {
        do {
            std::wstring wsExe(entry.szExeFile);
            std::string exeName(wsExe.begin(), wsExe.end());
            if (_stricmp(exeName.c_str(), name.c_str()) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

void start_injector_menu() {
    const std::string dll_folder = "dlls";
    if (!fs::exists(dll_folder)) {
        fs::create_directory(dll_folder);
    }

    while (true) {
        std::cout << "[1] Inject a DLL\n";
        std::cout << "Select an option: ";
        int option = -1;
        std::cin >> option;
        std::cin.ignore(10000, '\n');

        if (option == 0) {
            break;
        } else if (option == 1) {
            std::vector<fs::directory_entry> dlls;
            for (const auto& entry : fs::directory_iterator(dll_folder)) {
                if (entry.path().extension() == ".dll") {
                    dlls.push_back(entry);
                }
            }
            if (dlls.empty()) {
                std::cout << "No DLLs found in 'dlls' folder.\n";
                continue;
            }
            std::cout << "Available DLLs:\n";
            for (size_t i = 0; i < dlls.size(); ++i) {
                std::cout << "bvinjector: " << i+1 << ". " << dlls[i].path().filename().string() << '\n';
            }
            std::cout << "Select DLL number: ";
            int dll_choice = 0;
            std::cin >> dll_choice;
            std::cin.ignore(10000, '\n');
            if (dll_choice < 1 || dll_choice > (int)dlls.size()) {
                std::cout << "Invalid selection.\n";
                continue;
            }
            std::string dll_path = dlls[dll_choice-1].path().string();

            std::cout << "Enter the name of the process to inject into (e.g. notepad.exe): ";
            std::string proc_name;
            std::getline(std::cin, proc_name);

            DWORD pid = GetProcessIdByName(proc_name);
            if (pid == 0) {
                std::cout << "bvinjector: Process not found.\n";
                continue;
            }
            std::cout << "bvinjector: Process pid: " << pid << std::endl;

            HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
            if (!hProc) {
                std::cout << "bvinjector: OpenProcess failed: 0x" << std::hex << GetLastError() << std::dec << std::endl;
                continue;
            }

            std::ifstream File(dll_path, std::ios::binary | std::ios::ate);
            if (File.fail()) {
                std::cout << "bvinjector: Opening the file failed.\n";
                CloseHandle(hProc);
                continue;
            }
            auto FileSize = File.tellg();
            if (FileSize < 0x1000) {
                std::cout << "bvinjector: Filesize invalid.\n";
                File.close();
                CloseHandle(hProc);
                continue;
            }
            BYTE* pSrcData = new BYTE[(UINT_PTR)FileSize];
            if (!pSrcData) {
                std::cout << "bvinjector: Can't allocate dll file.\n";
                File.close();
                CloseHandle(hProc);
                continue;
            }
            File.seekg(0, std::ios::beg);
            File.read((char*)(pSrcData), FileSize);
            File.close();

            std::cout << "bvinjector: Mapping...\n";
            if (!ManualMapDll(hProc, pSrcData, FileSize)) {
                delete[] pSrcData;
                CloseHandle(hProc);
                std::cout << "Error while mapping.\n";
                std::cout << "Press enter to return to menu...";
                std::cin.get();
                clear_console();
                continue;
            }
            delete[] pSrcData;
            CloseHandle(hProc);
            std::cout << "Injection successful!\n";
            std::cout << "Press enter to return to menu...";
            std::cin.get();
            clear_console();
        } else {
            std::cout << "bvinjector: Invalid option.\n";
        }
    }
}

} // bvn
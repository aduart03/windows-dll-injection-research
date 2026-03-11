#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <fstream>
#pragma comment(lib, "Advapi32.lib")

struct ProcessMatch {
    DWORD pid;
    std::string name;
};

struct AccessTest {
    DWORD mask;
    const char* label;
};

bool IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;

    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(
            &ntAuthority,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &adminGroup)) {

        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    return isAdmin == TRUE;
}

const char* ErrorToString(DWORD err) {
    switch (err) {
        case 5: return "Access denied";
        case 87: return "Invalid parameter";
        default: return "Unknown error";
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: process_enum.exe <process_name>\n";
        return 1;
    }

    std::string target = argv[1];
    std::cout << "Running as admin: " << (IsRunningAsAdmin() ? "YES" : "NO") << "\n\n";
    std::vector<ProcessMatch> matches;

    std::ofstream logFile("..\\logs\\injection_results.txt", std::ios::app);

    if (!logFile) {
        std::cout << "Warning: could not open log file for writing.\n";
    }

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create snapshot. Error: " << GetLastError() << std::endl;
        return 1;
    }

    PROCESSENTRY32 process{};
    process.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &process)) {
        do {
            std::string exeName = process.szExeFile;

            if (_stricmp(exeName.c_str(), target.c_str()) == 0) {
                matches.push_back({ process.th32ProcessID, exeName });
            }

        } while (Process32Next(snapshot, &process));
    } else {
        std::cout << "Failed to enumerate processes. Error: " << GetLastError() << std::endl;
        CloseHandle(snapshot);
        return 1;
    }

    CloseHandle(snapshot);

    if (matches.empty()) {
        std::cout << "Target process not found.\n";
        return 1;
    }

    std::cout << "Found " << matches.size() << " matching process(es) for: " << target << "\n\n";
    if (logFile) {
        logFile << "Found " << matches.size() << " matching process(es) for: " << target << "\n\n";
    }

    AccessTest tests[] = {
        { PROCESS_QUERY_INFORMATION, "PROCESS_QUERY_INFORMATION" },
        { PROCESS_VM_READ, "PROCESS_VM_READ" },
        { PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, "QUERY_INFORMATION | VM_READ" },
        { PROCESS_VM_OPERATION, "PROCESS_VM_OPERATION" },
        { PROCESS_VM_WRITE, "PROCESS_VM_WRITE" },
        { PROCESS_CREATE_THREAD, "PROCESS_CREATE_THREAD" }
    };

    for (const auto& match : matches) {
        std::cout << "========================================\n";
        std::cout << "Process: " << match.name << "\n";
        std::cout << "PID:     " << match.pid << "\n";
        std::cout << "----------------------------------------\n";

        for (const auto& test : tests) {
            SetLastError(0);
            HANDLE hProcess = OpenProcess(test.mask, FALSE, match.pid);

            std::cout << std::left << std::setw(35) << test.label;

            if (hProcess != NULL) {
                std::cout << "SUCCESS\n";
                CloseHandle(hProcess);
            } else {
                DWORD err = GetLastError();
                std::cout << "FAILED (Error: " << err << " - " << ErrorToString(err) << ")\n";
            }
        }

        std::cout << std::endl;
    }

    return 0;
}


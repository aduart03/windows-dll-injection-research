#include "process_enum.h"

#include <tlhelp32.h>
#include <cstring>

std::vector<ProcessMatch> enumerate_all_processes() {
    std::vector<ProcessMatch> processes;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32 process{};
    process.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &process)) {
        do {
            std::string exeName = process.szExeFile;
            processes.push_back({ process.th32ProcessID, exeName });
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);
    return processes;
}

std::vector<ProcessMatch> enumerate_processes(const std::string& target) {
    std::vector<ProcessMatch> matches;
    auto all_processes = enumerate_all_processes();

    for (const auto& process : all_processes) {
        if (_stricmp(process.name.c_str(), target.c_str()) == 0) {
            matches.push_back(process);
        }
    }

    return matches;
}
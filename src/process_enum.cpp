#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

int main() {

    HANDLE snapshot;
    PROCESSENTRY32 process;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create snapshot\n";
        return 1;
    }

    process.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &process)) {

        do {
            std::wcout << L"PID: " << process.th32ProcessID
                       << L" | Name: " << process.szExeFile << std::endl;

        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    return 0;
}
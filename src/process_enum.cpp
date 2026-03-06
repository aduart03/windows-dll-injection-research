#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string> // for std::string and std::cout

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage : processs_enum.exe <process_name>\n";
        return 1;
    }

    std::string target = argv[1];
    DWORD targetPID = 0;

    // Snapshot processes in the system
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create snapshot. Error: \n" << GetLastError() << std::endl;
        return 1;
    }

    PROCESSENTRY32 process{};   // Initialize the PROCESSENTRY32 structure
    process.dwSize = sizeof(PROCESSENTRY32);

    // Iterate through the processes and print their IDs and names
    if (Process32First(snapshot, &process)) {

        // Check if the process name matches the target
        if (!Process32First(snapshot, &process)) {
        std::cout << "Failed to enumerate processes. Error: " << GetLastError() << std::endl;
        }

        // Iterate through the processes and check for the target process
        do {
            std::string exeName = process.szExeFile;
            if (exeName == target) {
                targetPID = process.th32ProcessID;
                std::cout << "Found target process: " << exeName << " | PID: " << targetPID << std::endl;
                break;
            }

        } while (Process32Next(snapshot, &process));
    } else {
        std::cout << "Failed to enumerate processes. Error \n" << GetLastError() << std::endl;
    }

    // Close the snapshot handle
    CloseHandle(snapshot);

    if (targetPID == 0) {
        std::cout << "Target process not found.\n";
        return 1;
    }

    std::cout << "Found target PID: " << targetPID << "\n";

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, targetPID);
    if (hProcess == NULL) {
        std::cout << "OpenProcess failed. Error: " << GetLastError() << "\n";
        return 1;
    }

    std::cout << "OpenProcess succeeded for PID " << targetPID << "\n";
    CloseHandle(hProcess);



    return 0;
}
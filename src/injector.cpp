#include "injector.h"
#include <windows.h>

InjectionResult inject_dll(DWORD pid, const std::string& dll_path) {
    InjectionResult result{};
    result.pid = pid;
    result.dll_path = dll_path;
    result.success = false;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        result.message = "OpenProcess failed";
        return result;
    }

    LPVOID remote_mem = VirtualAllocEx(
        hProcess,
        nullptr,
        dll_path.size() + 1,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!remote_mem) {
        result.message = "VirtualAllocEx failed";
        CloseHandle(hProcess);
        return result;
    }

    if (!WriteProcessMemory(
            hProcess,
            remote_mem,
            dll_path.c_str(),
            dll_path.size() + 1,
            nullptr)) {
        result.message = "WriteProcessMemory failed";
        VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return result;
    }

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) {
        result.message = "GetModuleHandle failed";
        CloseHandle(hProcess);
        return result;
    }

    FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");
    if (!loadLibraryAddr) {
        result.message = "GetProcAddress failed";
        CloseHandle(hProcess);
        return result;
    }

    HANDLE hThread = CreateRemoteThread(
        hProcess,
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)loadLibraryAddr,
        remote_mem,
        0,
        nullptr
    );

    if (!hThread) {
        result.message = "CreateRemoteThread failed";
        CloseHandle(hProcess);
        return result;
    }

    WaitForSingleObject(hThread, INFINITE);

    result.success = true;
    result.message = "Injection successful";

    CloseHandle(hThread);
    CloseHandle(hProcess);

    return result;
}
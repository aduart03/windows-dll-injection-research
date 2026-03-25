#pragma once

#include <string>
#include <windows.h>

struct InjectionResult {
    DWORD pid;
    std::string dll_path;
    bool success;
    std::string message;
};

InjectionResult inject_dll(DWORD pid, const std::string& dll_path);
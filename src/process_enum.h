#pragma once

#include <windows.h>
#include <string>
#include <vector>

struct ProcessMatch {
    DWORD pid;
    std::string name;
};

std::vector<ProcessMatch> enumerate_all_processes();
std::vector<ProcessMatch> enumerate_processes(const std::string& target);
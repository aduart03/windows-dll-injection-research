#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "process_enum.h"

struct AccessTest {
    DWORD mask;
    const char* label;
};

struct AccessOutcome {
    std::string label;
    bool success;
    DWORD error_code;
};

struct AccessCheckResult {
    DWORD pid;
    std::string process_name;
    std::vector<AccessOutcome> outcomes;
};

AccessCheckResult check_process_access(const ProcessMatch& match);
const char* error_to_string(DWORD err);
bool is_running_as_admin();
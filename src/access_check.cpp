#include "access_check.h"

#pragma comment(lib, "Advapi32.lib")

bool is_running_as_admin() {
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

const char* error_to_string(DWORD err) {
    switch (err) {
        case 0: return "Success";
        case 5: return "Access denied";
        case 87: return "Invalid parameter";
        default: return "Unknown error";
    }
}

AccessCheckResult check_process_access(const ProcessMatch& match) {
    AccessCheckResult result{};
    result.pid = match.pid;
    result.process_name = match.name;

    AccessTest tests[] = {
        { PROCESS_QUERY_INFORMATION, "PROCESS_QUERY_INFORMATION" },
        { PROCESS_VM_READ, "PROCESS_VM_READ" },
        { PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, "QUERY_INFORMATION | VM_READ" },
        { PROCESS_VM_OPERATION, "PROCESS_VM_OPERATION" },
        { PROCESS_VM_WRITE, "PROCESS_VM_WRITE" },
        { PROCESS_CREATE_THREAD, "PROCESS_CREATE_THREAD" }
    };

    for (const auto& test : tests) {
        SetLastError(0);
        HANDLE hProcess = OpenProcess(test.mask, FALSE, match.pid);

        AccessOutcome outcome{};
        outcome.label = test.label;

        if (hProcess != NULL) {
            outcome.success = true;
            outcome.error_code = 0;
            CloseHandle(hProcess);
        } else {
            outcome.success = false;
            outcome.error_code = GetLastError();
        }

        result.outcomes.push_back(outcome);
    }

    return result;
}
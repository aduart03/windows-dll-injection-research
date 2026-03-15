#include <iostream>
#include <string>
#include <vector>

#include "process_enum.h"
#include "access_check.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    std::vector<ProcessMatch> processes;
    bool all_mode = false;

    std::cout << "Running as admin: "
              << (is_running_as_admin() ? "YES" : "NO")
              << "\n\n";

    if (argc == 1) {
        processes = enumerate_all_processes();
        all_mode = true;
        std::cout << "Testing ALL running processes.\n\n";
    } else if (argc == 2) {
        std::string target = argv[1];
        processes = enumerate_processes(target);

        if (processes.empty()) {
            std::cout << "Target process not found.\n";
            return 1;
        }

        std::cout << "Found " << processes.size()
                  << " matching process(es) for: " << target << "\n\n";
    } else {
        std::cout << "Usage:\n";
        std::cout << "  main.exe                -> test all processes\n";
        std::cout << "  main.exe <process_name> -> test one process name\n";
        return 1;
    }

    std::string txt_log_path = "..\\logs\\injection_results.txt";
    std::string csv_log_path = "..\\logs\\access_results.csv";

    write_csv_header_if_needed(csv_log_path);

    for (const auto& proc : processes) {
        auto result = check_process_access(proc);

        if (!all_mode) {
            print_access_result(result);
        }

        append_access_result_to_log(result, txt_log_path);
        append_access_result_to_csv(result, csv_log_path);
    }

    if (all_mode) {
        std::cout << "Finished testing " << processes.size()
                  << " processes.\nResults written to:\n"
                  << "  " << txt_log_path << "\n"
                  << "  " << csv_log_path << "\n";
    }

    return 0;
}
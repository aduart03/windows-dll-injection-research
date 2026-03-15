#include "logger.h"

#include <fstream>
#include <iomanip>
#include <iostream>

void print_access_result(const AccessCheckResult& result) {
    std::cout << "========================================\n";
    std::cout << "Process: " << result.process_name << "\n";
    std::cout << "PID:     " << result.pid << "\n";
    std::cout << "----------------------------------------\n";

    for (const auto& outcome : result.outcomes) {
        std::cout << std::left << std::setw(35) << outcome.label;

        if (outcome.success) {
            std::cout << "SUCCESS\n";
        } else {
            std::cout << "FAILED (Error: " << outcome.error_code
                      << " - " << error_to_string(outcome.error_code) << ")\n";
        }
    }

    std::cout << std::endl;
}

void append_access_result_to_log(const AccessCheckResult& result, const std::string& log_path) {
    std::ofstream logFile(log_path, std::ios::app);

    if (!logFile) {
        return;
    }

    logFile << "========================================\n";
    logFile << "Process: " << result.process_name << "\n";
    logFile << "PID:     " << result.pid << "\n";
    logFile << "----------------------------------------\n";

    for (const auto& outcome : result.outcomes) {
        logFile << std::left << std::setw(35) << outcome.label;

        if (outcome.success) {
            logFile << "SUCCESS\n";
        } else {
            logFile << "FAILED (Error: " << outcome.error_code
                    << " - " << error_to_string(outcome.error_code) << ")\n";
        }
    }

    logFile << "\n";
}

void write_csv_header_if_needed(const std::string& csv_path) {
    std::ifstream inFile(csv_path);
    if (inFile.good()) {
        return;
    }

    std::ofstream outFile(csv_path, std::ios::app);
    if (!outFile) {
        return;
    }

    outFile << "pid,process_name,access_label,success,error_code,error_message\n";
}

void append_access_result_to_csv(const AccessCheckResult& result, const std::string& csv_path) {
    std::ofstream csvFile(csv_path, std::ios::app);

    if (!csvFile) {
        return;
    }

    for (const auto& outcome : result.outcomes) {
        csvFile << result.pid << ","
                << result.process_name << ","
                << outcome.label << ","
                << (outcome.success ? 1 : 0) << ","
                << outcome.error_code << ","
                << "\"" << error_to_string(outcome.error_code) << "\""
                << "\n";
    }
}
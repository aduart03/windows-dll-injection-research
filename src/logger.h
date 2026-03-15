#pragma once

#include <string>
#include <vector>

#include "access_check.h"

void print_access_result(const AccessCheckResult& result);
void append_access_result_to_log(const AccessCheckResult& result, const std::string& log_path);
void append_access_result_to_csv(const AccessCheckResult& result, const std::string& csv_path);
void write_csv_header_if_needed(const std::string& csv_path);
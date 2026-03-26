#pragma once

#include <test_config.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

constexpr std::string test_file_path(const std::string& file) {
    return fs::path(TEST_FILES_PATH) / file;
}
std::string run_lsbin_test(const fs::path& file);

#pragma once

#include <file.h>
#include <stdlib.h>
#include <test_config.h>
#include <filesystem>
#include <json.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace nlohmann;

using Fmt = ExecFile::Type::Fmt;
using Arch = ExecFile::Type::Arch;

constexpr std::string test_file_path(const std::string& file) {
    return fs::path(TEST_FILES_PATH) / file;
}

std::optional<std::vector<ExecFile>> run_lsbin_test(const fs::path& file);
bool has_lib(const ExecFile& file, const std::string& s);

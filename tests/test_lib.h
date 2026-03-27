#pragma once

#include <test_config.h>
#include <exception>
#include <filesystem>
#include <format>
#include <string>
#include <vector>
#include <json.hpp>
#include <printr.h>
#include <stdlib.h>

namespace fs = std::filesystem;
using namespace nlohmann;

struct ExecFile {
    std::string path;
    struct Type {
        enum Fmt {
            PE, 
            ELF,
            MACH_O, 
            FAT_MACH_O
        } format;

        enum Arch {
            A32, A64
        } arch;
    } type;

    struct Info {
        std::string interp;
        std::vector<std::string> libraries;
    } info;
};
void from_json(const json& j, ExecFile& file);

class FileDecodeException : public std::exception {
public:
    FileDecodeException(std::string error, std::string got) 
        : msg(std::format("Failed to decode string \"{}\": {}", error, got)) {}

    const char* what() const noexcept override {
        return msg.c_str();
    }
private:
    std::string msg;
};

using Fmt = ExecFile::Type::Fmt;
using Arch = ExecFile::Type::Arch;

constexpr std::string test_file_path(const std::string& file) {
    return fs::path(TEST_FILES_PATH) / file;
}

std::optional<std::vector<ExecFile>> run_lsbin_test(const fs::path& file);
bool has_lib(const ExecFile& file, const std::string& s);

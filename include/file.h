#pragma once
// common structs for files

#include <json.hpp>
#include <string>
#include <vector>

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
            A32,
            A64
        } arch;
    } type;

    struct Info {
        std::string interp;
        std::vector<std::string> libraries;
    } info;
};

void to_json(nlohmann::json& j, const ExecFile& e);
#include <mains.h>
#include <file.h>
#include <json.hpp>
#include <printr.h>
#include <format>

using namespace nlohmann;

void output_json_main(exe_vec res) {
    printer::println("{}", ((json)res).dump(4));
}

void output_text_main(exe_vec res) {
    using Fmt = ExecFile::Type::Fmt;
    using Arch = ExecFile::Type::Arch;

    const auto type_string = [](const ExecFile& file) -> std::string {
        std::string format, arch;
        switch (file.type.format) {
            case Fmt::ELF:
                format = "ELF"; break;
            case Fmt::PE:
                format = "PE"; break;
            case Fmt::MACH_O:
                format = "MachO"; break;
            case Fmt::FAT_MACH_O:
                format = "FAT MachO"; break;
        }

        switch (file.type.arch) {
            case Arch::A32:
                arch = "32"; break;
            case Arch::A64:
                arch = "64"; break;
        }

        return std::format("{}({})", format, arch);
    };

    enum FieldType {
        INTERP, LIBRARY
    };

    const auto format_field = [](
        const ExecFile& file, 
        const std::string& field, 
        FieldType type
    ) -> std::string {
        std::string pre_string;
        switch (file.type.format) {
            case Fmt::PE: {
                switch (type) {
                    case INTERP: pre_string = ""; break; // PE doesnt have interp
                    case LIBRARY: pre_string = "Imported DLL"; break;
                }
                break;
            }
            case Fmt::ELF: {
                switch (type) {
                    case INTERP: pre_string = "Program Interpreter"; break; // PE doesnt have interp
                    case LIBRARY: pre_string = "Needed Library"; break;
                }
                break;
            }

            case Fmt::MACH_O:
            case Fmt::FAT_MACH_O: {
                switch (type) {
                    case INTERP: pre_string = "Load Dylinker"; break; // PE doesnt have interp
                    case LIBRARY: pre_string = "Load Dylib"; break;
                }
                break;
            }
        }

        return std::format("{}: {}", pre_string, field);
    };

    for (const auto& file : res) {
        printer::println("{} File: {}", type_string(file), file.path);
        if (file.type.format != Fmt::PE) {
            printer::println("\t{}", format_field(file, file.info.interp, INTERP));
        }

        for (const auto& lib : file.info.libraries) {
            printer::println("\t{}", format_field(file, lib, LIBRARY));
        }
    }
}

void output(exe_vec res, PrintFormat fmt) {
    switch (fmt) {
        case PrintFormat::TEXT:
            return output_text_main(res);
        case PrintFormat::JSON:
            return output_json_main(res);
    }
}
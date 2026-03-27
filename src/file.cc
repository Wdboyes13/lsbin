#include <file.h>

using namespace nlohmann;
using Fmt = ExecFile::Type::Fmt;
using Arch = ExecFile::Type::Arch;

std::string format_to_text(Fmt fmt) {
    switch (fmt) {
        case Fmt::PE:
            return "pe";
        case Fmt::ELF:
            return "elf";
        case Fmt::MACH_O:
            return "macho";
        case Fmt::FAT_MACH_O:
            return "fatmacho";
    }
}

std::string arch_to_text(Arch arch) {
    switch (arch) {
        case Arch::A32:
            return "32";
        case Arch::A64:
            return "64";
    }
}

void to_json(nlohmann::json& j, const ExecFile& e) {
    j = json({
        {"file", {
            {"path", e.path},
            {"format", format_to_text(e.type.format)},
            {"arch", arch_to_text(e.type.arch)}
        }},
        {"info", {
            {"interp", e.info.interp},
            {"libraries", e.info.libraries}
        }}
    });
}
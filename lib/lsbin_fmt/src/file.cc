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
    auto file_obj = json{
        { "path", e.path },
        { "format", format_to_text(e.type.format) },
        { "arch", arch_to_text(e.type.arch) }
    };

    auto info_obj = json{
        { "interp", e.info.interp },
        { "libraries", e.info.libraries }
    };

    j = json({ { "file", file_obj },
               { "info", info_obj } });
}

std::optional<Fmt> text_to_format(std::string text) {
    if (text == "pe") {
        return Fmt::PE;
    } else if (text == "elf") {
        return Fmt::ELF;
    } else if (text == "macho") {
        return Fmt::MACH_O;
    } else if (text == "fatmacho") {
        return Fmt::FAT_MACH_O;
    } else {
        return std::nullopt;
    }
}

std::optional<Arch> text_to_arch(std::string text) {
    if (text == "32") {
        return Arch::A32;
    } else if (text == "64") {
        return Arch::A64;
    } else {
        return std::nullopt;
    }
}

void from_json(const json& j, ExecFile& file) {
    j.at("file").at("path").get_to(file.path);

    std::string fmt_str, arch_str;
    j.at("file").at("format").get_to(fmt_str);
    j.at("file").at("arch").get_to(arch_str);

    std::optional<Arch> arch;
    std::optional<Fmt> fmt;

    if (!(arch = text_to_arch(arch_str))) {
        throw FileDecodeException("expected one of: \"64\", \"32\"", arch_str);
    }

    if (!(fmt = text_to_format(fmt_str))) {
        throw FileDecodeException("expected one of: \"pe\", \"elf\", \"macho\", \"fatmacho\"", fmt_str);
    }

    file.type.arch = arch.value();
    file.type.format = fmt.value();

    j.at("info").at("interp").get_to(file.info.interp);
    j.at("info").at("libraries").get_to(file.info.libraries);
}
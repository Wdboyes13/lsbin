#include <test_lib.h>
#include <test_config.h>
#include <cstdlib>

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

std::optional<std::vector<ExecFile>> run_lsbin_test(const fs::path& file) {
    std::string cmd = std::string(LSBIN_PATH) + " --json " + test_file_path(file);
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return std::nullopt;
    }

    std::string result;
    char buf[128];

    while (fgets(buf, sizeof(buf), pipe)) {
        result += buf;
    }

    pclose(pipe);
    return json::parse(result).get<std::vector<ExecFile>>();
}


bool has_lib(const ExecFile& file, const std::string& s) {
    auto& libs = file.info.libraries;
    return std::find(libs.begin(), libs.end(), s) != libs.end();
}
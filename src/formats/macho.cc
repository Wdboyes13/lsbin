#include <file.h>
#include <formats/macho.h>
#include <mag.h>
#include <mains.h>
#include <printr.h>
#include <filesystem>

#define MHDR32SZ 28
#define MHDR64SZ 32

std::optional<ExecFile> process_macho(uint32_t sig, uint32_t ncmds, uchar* data, bool fat = false) {
    uchar* lcmds;
    ExecFile file{};

    if (fat) {
        file.type.format = ExecFile::Type::FAT_MACH_O;
    } else {
        file.type.format = ExecFile::Type::MACH_O;
    }

    if (sig == MAGIC_MH64 || sig == RMAGIC_MH64) {
        file.type.arch = ExecFile::Type::A64;
        lcmds = data + MHDR64SZ;
    } else if (sig == MAGIC_MH32 || sig == RMAGIC_MH32) {
        file.type.arch = ExecFile::Type::A32;
        lcmds = data + MHDR32SZ;
    } else {
        return std::nullopt;
    }

    for (int i = 0; i < ncmds; i++) {
        auto cmd = (load_command*)lcmds;
        if (cmd->cmd == LC_LOAD_DYLIB) {
            auto dcmd = (dylib_command*)cmd;
            file.info.libraries.push_back(((char*)cmd) + dcmd->dylib.name.offset);
        } else if (cmd->cmd == LC_LOAD_DYLINKER) {
            auto dcmd = (dylinker_command*)cmd;
            file.info.interp = ((char*)cmd) + dcmd->name.offset;
        }
        lcmds += cmd->cmdsize;
    }

    return file;
}

exefn_result lsbin_machomain(uchar* data, const char* fname) {
    uint32_t sig = *(uint32_t*)data;
    std::vector<ExecFile> ret;
    fat_header fhdr = *(fat_header*)data;
    auto nfat_arch = swap_endian(fhdr.nfat_arch);

    if (sig == MAGIC_MF32 || sig == RMAGIC_MF32 || sig == MAGIC_MF64 ||
        sig == RMAGIC_MF64) {
        auto process_fat = [&](auto* farch) -> exefn_result {
            for (int i = 0; i < nfat_arch; i++) {
                auto offset = swap_endian(farch[i].offset);
                auto tsig = *(uint32_t*)&data[offset];
                auto tncmds = *(uint32_t*)&data[offset + 16];

                auto pret = process_macho(tsig, tncmds, (uchar*)&data[offset], true);
                if (pret.has_value()) {
                    pret.value().path = std::filesystem::canonical(fname).string() + ":" + std::to_string(i);
                    ret.push_back(pret.value());
                } else {
                    return std::nullopt;
                }
            }
            return ret;
        };

        if (sig == MAGIC_MF32 || sig == RMAGIC_MF32) {
            return process_fat((fat_arch*)&data[sizeof(fat_header)]);
        } else {
            return process_fat((fat_arch_64*)&data[sizeof(fat_header)]);
        }
    } else {
        auto pret = process_macho(sig, *(uint32_t*)&data[16], data);
        if (pret.has_value()) {
            pret.value().path = std::filesystem::canonical(fname);
            ret.push_back(pret.value());
        } else {
            return std::nullopt;
        }
    }

    return ret;
}

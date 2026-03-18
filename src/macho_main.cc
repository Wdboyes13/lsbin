#include <macho.h>
#include <mag.h>
#include <mains.h>
#include <printr.h>

#define MHDR32SZ 28
#define MHDR64SZ 32

int process_macho(uint32_t sig, uint32_t ncmds, uchar* data) {
    uchar* lcmds;

    // 64bit magic = 0xfeedfacf, 32bit = 0xfeedface
    if (sig == MAGIC_MH64 || sig == RMAGIC_MH64) {
        lcmds = data + MHDR64SZ;
    } else if (sig == MAGIC_MH32 || sig == RMAGIC_MH32) {
        lcmds = data + MHDR32SZ;
    } else {
        printer::eprintln("Unknown file got magic: 0x{:08x}", sig);
        return 1;
    }

    for (int i = 0; i < ncmds; i++) {
        auto cmd = (load_command*)lcmds;
        if (cmd->cmd == LC_LOAD_DYLIB) {
            auto dcmd = (dylib_command*)cmd;
            auto name = ((char*)cmd) + dcmd->dylib.name.offset;
            printer::println("Load dylib: {}", name);
        } else if (cmd->cmd == LC_LOAD_DYLINKER) {
            auto dcmd = (dylinker_command*)cmd;
            auto name = ((char*)cmd) + dcmd->name.offset;
            printer::println("Load dylinker: {}", name);
        }
        lcmds += cmd->cmdsize;
    }

    return 0;
}

int lsbin_machomain(uchar* data) {
    uint32_t sig = *(uint32_t*)data;
    int ret = 0;
    fat_header fhdr = *(fat_header*)data;
    auto nfat_arch = swap_endian(fhdr.nfat_arch);

    if (sig == MAGIC_MF32 || sig == RMAGIC_MF32 || sig == MAGIC_MF64 ||
        sig == RMAGIC_MF64) {
        auto process_fat = [&](auto* farch) {
            for (int i = 0; i < nfat_arch; i++) {
                printer::println("FAT Index: {}", i + 1);
                auto offset = swap_endian(farch[i].offset);
                auto tsig = *(uint32_t*)&data[offset];
                auto tncmds = *(uint32_t*)&data[offset + 16];

                ret = process_macho(tsig, tncmds, (uchar*)&data[offset]);

                if (ret != 0) {
                    return 1;
                }
            }
            return 0;
        };

        if (sig == MAGIC_MF32 || sig == RMAGIC_MF32) {
            return process_fat((fat_arch*)&data[sizeof(fat_header)]);
        } else {
            return process_fat((fat_arch_64*)&data[sizeof(fat_header)]);
        }
    } else {
        return process_macho(sig, *(uint32_t*)&data[16], data);
    }

    return ret;
}

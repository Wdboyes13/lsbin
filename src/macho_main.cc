#include <macho.h>
#include <mag.h>
#include <mains.h>
#include <printr.h>

#define MHDR32SZ 28
#define MHDR64SZ 32

int lsbin_machomain(uchar* data) {
    uchar* lcmds;
    uint32_t sig = *(uint32_t*)data;
    uint32_t ncmds = *(uint32_t*)&data[16];

    // 64bit magic = 0xfeedfacf, 32bit = 0xfeedface
    if (sig == MAGIC_MH64 || sig == RMAGIC_MH64) {
        lcmds = data + MHDR64SZ;
    } else if (sig == MAGIC_MH32 || sig == RMAGIC_MH32) {
        lcmds = data + MHDR32SZ;
    } else {
        printer::eprintln("Unknown file");
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

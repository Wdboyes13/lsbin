#include <mach-o/loader.h>
#include <mag.h>
#include <mains.h>
#include <printr.h>

int lsbin_machomain(uchar* data) {
    uint32_t ncmds;
    uchar* lcmds;

    uint32_t sig = *(uint32_t*)data;

    // 64bit magic = 0xfeedfacf, 32bit = 0xfeedface
    if (sig == MAGIC_MH64 || sig == RMAGIC_MH64) {
        auto hdr = (mach_header_64*)data;
        ncmds = hdr->ncmds;
        lcmds = data + sizeof(*hdr);
    } else if (sig == MAGIC_MH32 || sig == RMAGIC_MH32) {
        auto hdr = (mach_header*)data;
        ncmds = hdr->ncmds;
        lcmds = data + sizeof(*hdr);
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

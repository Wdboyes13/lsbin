#include <mach-o/loader.h>
#include <printr.h>

#define VALIDMACHO(PDATA) (((uint32_t)*PDATA) == MH_CIGAM_64)

int lsbin_machomain(char* data) {
    mach_header_64* hdr = (mach_header_64*)data;

    char* lcmds = data + ((sizeof(mach_header_64) + 7) & ~7);

    for (int i = 0; i < hdr->ncmds; i++) {
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

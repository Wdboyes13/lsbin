#include <mains.h>
#include <printr.h>
#include <filesystem>
#include <fstream>
#include "__winapi/defs.h"

#include <elf.h>
#include <mach-o/loader.h>
#include <windows.h>

uint32_t __bswap32(uint32_t x) {
    return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

uint16_t __bswap16(uint16_t x) { return (x << 8) | (x >> 8); }

int main(int ac, char** av) {
    if (ac != 2) {
        printer::eprintln("Expected filename argument\n{} <FILE>", av[0]);
        return 1;
    }

    auto fname = av[1];
    auto fsize = std::filesystem::file_size(fname);

    std::ifstream file(fname);
    if (!file) {
        printer::eprintln("Failed to open file");
        return 1;
    }

    char* data = new char[fsize];
    file.read(data, fsize);
    file.close();

    int ret;

    auto sig = (uint32_t*)data;

    if (*sig == MH_MAGIC_64 || __bswap32(*sig) == MH_MAGIC_64) {
        printer::println("Mach-O File");
        ret = lsbin_machomain(data);
    } else if (*sig == 0x7f454c46 || __bswap32(*sig) == 0x7f454c46) {
        printer::println("ELF File");
        ret = lsbin_elfmain(data);
    } else if ((uint16_t)*sig == IMAGE_DOS_SIGNATURE ||
               __bswap16((uint16_t)*sig) == IMAGE_DOS_SIGNATURE) {
        printer::println("PE File");
        ret = lsbin_pemain(data);
    } else {
        printer::eprintln("Unknown file type: Got magic 0x{:x}", *sig);
        ret = 1;
    }

    delete[] data;

    return ret;
}

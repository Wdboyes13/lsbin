#include <mag.h>
#include <mains.h>
#include <printr.h>

#include <filesystem>
#include <fstream>

#include <elf.h>
#include <mach-o/loader.h>

int main(int ac, char** av) {
    if (ac != 2) {
        printer::eprintln("Expected filename argument\n{} <FILE>", av[0]);
        return 1;
    }

    auto fname = av[1];
    auto fsize = std::filesystem::file_size(fname);

    if (fsize < 4) {
        printer::eprintln(
            "This file couldn't possibly be an executable\n"
            "It's only {} bytes we need 4 bytes to check file type",
            fsize);
        return 1;
    }

    std::ifstream file(fname);
    if (!file) {
        printer::eprintln("Failed to open file");
        return 1;
    }

    uchar* data = new uchar[fsize];
    file.read((char*)data, fsize);
    file.close();

    int ret;

    uint32_t sig32 = *(uint32_t*)data;
    uint16_t sig16 = *(uint16_t*)data;

    if (sig32 == MAGIC_MH64 || sig32 == RMAGIC_MH64 || sig32 == MAGIC_MH32 ||
        sig32 == RMAGIC_MH32) {
        printer::println("Mach-O File");
        ret = lsbin_machomain(data);
    } else if (sig32 == MAGIC_ELF || sig32 == RMAGIC_ELF) {
        printer::println("ELF File");
        if (fsize < EI_NIDENT) {
            printer::eprintln("Thought this was ELF but not large enough to "
                              "store a full E_IDENT so it can't be");
            delete[] data;
            return 1;
        }
        if (data[EI_CLASS] == ELFCLASS32) {
            ret = lsbin_elf32main(data);
        } else if (data[EI_CLASS] == ELFCLASS64) {
            ret = lsbin_elf64main(data);
        } else {
            printer::eprintln("Unknown ELF class");
        }
    } else if (sig16 == MAGIC_DOS || sig16 == RMAGIC_DOS) {
        printer::println("PE File");
        ret = lsbin_pemain(data);
    } else {
        printer::eprintln("Unknown file type: Got magic 0x{:08x}", sig32);
        ret = 1;
    }

    delete[] data;

    return ret;
}

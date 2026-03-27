#include <mag.h>
#include <mains.h>
#include <printr.h>
#include <cstdint>
#include <fstream>
#include <optional>

#include <formats/elf.h>

bool is_macho(uint32_t sig) {
    return (sig == MAGIC_MH64 || sig == RMAGIC_MH64 || sig == MAGIC_MH32 ||
            sig == RMAGIC_MH32 || sig == MAGIC_MF64 || sig == RMAGIC_MF64 ||
            sig == MAGIC_MF32 || sig == RMAGIC_MF32);
}

exefn_result process_image(const char* fname) {
    std::ifstream file(fname, std::ios::binary);
    if (!file) {
        printer::eprintln("Failed to open file");
        return std::nullopt;
    }

    std::vector<uchar> data{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()};

    file.close();

    if (data.size() < 4) {
        printer::eprintln(
            "This file couldn't possibly be an executable\n"
            "It's only {} bytes we need 4 bytes to check file type",
            data.size());
        return std::nullopt;
    }

    exefn_result ret;
    uint32_t sig32 = *(uint32_t*)data.data();
    uint16_t sig16 = *(uint16_t*)data.data();

    if (is_macho(sig32)) {
        ret = lsbin_machomain(data.data(), fname);
    } else if (sig32 == MAGIC_ELF || sig32 == RMAGIC_ELF) {
        if (data.size() < EI_NIDENT) {
            printer::eprintln("Thought this was ELF but not large enough to "
                              "store a full E_IDENT so it can't be");
            return std::nullopt;
        }
        if (data[EI_CLASS] == ELFCLASS32) {
            ret = lsbin_elf32main(data.data(), fname);
        } else if (data[EI_CLASS] == ELFCLASS64) {
            ret = lsbin_elf64main(data.data(), fname);
        } else {
            printer::eprintln("Unknown ELF class");
            return std::nullopt;
        }
    } else if (sig16 == MAGIC_DOS || sig16 == RMAGIC_DOS) {
        ret = lsbin_pemain(data.data(), fname);
    } else {
        printer::eprintln("Unknown file type: Got magic 0x{:08x}", sig32);
        return std::nullopt;
    }

    return ret;
}
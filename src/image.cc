#include <mag.h>
#include <mains.h>
#include <printr.h>
#include <cstdint>
#include <fstream>
#include <optional>
#include <vector>
#include <iterator>

#include <formats/elf.h>

bool is_macho(uint32_t sig) {
    return (sig == MAGIC_MH64 || sig == RMAGIC_MH64 || sig == MAGIC_MH32 ||
            sig == RMAGIC_MH32 || sig == MAGIC_MF64 || sig == RMAGIC_MF64 ||
            sig == MAGIC_MF32 || sig == RMAGIC_MF32);
}

// Helper to read the entire file if needed (as the backend libraries expect raw memory buffers)
// but defers the allocation until the signature has been validated.
exefn_result process_image(const char* fname) {
    std::ifstream file(fname, std::ios::binary | std::ios::ate);
    if (!file) {
        printer::eprintln("Failed to open file");
        return std::nullopt;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size < 4) {
        printer::eprintln(
            "This file couldn't possibly be an executable\n"
            "It's only {} bytes we need 4 bytes to check file type",
            size);
        return std::nullopt;
    }

    // Read only the first 4 bytes to check format without allocating the whole file
    uint32_t sig32 = 0;
    uint16_t sig16 = 0;
    file.read(reinterpret_cast<char*>(&sig32), sizeof(sig32));
    // Reset back for the full file reading
    file.seekg(0, std::ios::beg);
    sig16 = static_cast<uint16_t>(sig32);

    exefn_result ret;

    // Check type before reading the rest of the binary
    if (is_macho(sig32) || sig32 == MAGIC_ELF || sig32 == RMAGIC_ELF || sig16 == MAGIC_DOS || sig16 == RMAGIC_DOS) {
        // Read the full binary data into buffer for parser consumption
        std::vector<uchar> data(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        
        file.close();

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
        }
    } else {
        printer::eprintln("Unknown file type: Got magic 0x{:08x}", sig32);
        file.close();
        return std::nullopt;
    }

    return ret;
}

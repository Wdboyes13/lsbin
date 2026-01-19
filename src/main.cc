#include <mains.h>
#include <printr.h>
#include <filesystem>
#include <fstream>

#include <elf.h>
#include <mach-o/loader.h>
#include <windows.h>

template<int SZ>
struct check_magic {
    check_magic(int x, int mag) {
        if (SZ == 16) {
            auto v = (uint16_t)x;
            if (v == mag || swap16(v) == mag) {
                good = true;
            }
        } else if (SZ == 32) {
            auto v = (uint32_t)x;
            if (v == mag || swap32(v) == mag) {
                good = true;
            }
        } else {
            good = false;
        }
    }

    bool good;

  private:
    uint16_t swap16(uint16_t x) { return (x << 8) | (x >> 8); }
    uint32_t swap32(uint32_t x) {
        return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) |
               (x << 24);
    }
};

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

    auto sig = (uint32_t)*data;

    if (check_magic<32>(sig, MH_MAGIC_64).good) {
        printer::println("Mach-O File");
        ret = lsbin_machomain(data);
    } else if (check_magic<32>(sig, 0x7f454c46).good) {
        printer::println("ELF File");
        ret = lsbin_elfmain(data);
    } else if (check_magic<16>(sig, IMAGE_DOS_SIGNATURE).good) {
        printer::println("PE File");
        ret = lsbin_pemain(data);
    } else {
        printer::eprintln("Unknown file type: Got magic 0x{:x}", sig);
        ret = 1;
    }

    delete[] data;

    return ret;
}

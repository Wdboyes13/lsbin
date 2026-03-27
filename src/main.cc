#include <mag.h>
#include <mains.h>
#include <printr.h>

#include <filesystem>
#include <fstream>

#include <formats/elf.h>
#include <formats/macho.h>

const char* license = 
"MIT License\n\n"

"Copyright (c) 2026 Wdboyes13\n\n"

"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
"of this software and associated documentation files (the \"Software\"), to deal\n"
"in the Software without restriction, including without limitation the rights\n"
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
"copies of the Software, and to permit persons to whom the Software is\n"
"furnished to do so, subject to the following conditions:\n\n"

"The above copyright notice and this permission notice shall be included in all\n"
"copies or substantial portions of the Software.\n\n"

"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
"SOFTWARE.\n";

bool is_macho(uint32_t sig) {
    return (sig == MAGIC_MH64 || sig == RMAGIC_MH64 || sig == MAGIC_MH32 ||
            sig == RMAGIC_MH32 || sig == MAGIC_MF64 || sig == RMAGIC_MF64 ||
            sig == MAGIC_MF32 || sig == RMAGIC_MF32);
}

exefn_result process_image(char* fname) {
    auto fsize = std::filesystem::file_size(fname);

    if (fsize < 4) {
        printer::eprintln(
            "This file couldn't possibly be an executable\n"
            "It's only {} bytes we need 4 bytes to check file type",
            fsize);
        return std::nullopt;
    }

    std::ifstream file(fname);
    if (!file) {
        printer::eprintln("Failed to open file");
        return std::nullopt;
    }

    exefn_result ret;

    uchar* data = new uchar[fsize];
    file.read((char*)data, fsize);
    file.close();

    uint32_t sig32 = *(uint32_t*)data;
    uint16_t sig16 = *(uint16_t*)data;

    if (is_macho(sig32)) {
        ret = lsbin_machomain(data, fname);
    } else if (sig32 == MAGIC_ELF || sig32 == RMAGIC_ELF) {
        if (fsize < EI_NIDENT) {
            printer::eprintln("Thought this was ELF but not large enough to "
                              "store a full E_IDENT so it can't be");
            delete[] data;
            return std::nullopt;
        }
        if (data[EI_CLASS] == ELFCLASS32) {
            ret = lsbin_elf32main(data, fname);
        } else if (data[EI_CLASS] == ELFCLASS64) {
            ret = lsbin_elf64main(data, fname);
        } else {
            printer::eprintln("Unknown ELF class");
            delete[] data;
            return std::nullopt;
        }
    } else if (sig16 == MAGIC_DOS || sig16 == RMAGIC_DOS) {
        ret = lsbin_pemain(data, fname);
    } else {
        printer::eprintln("Unknown file type: Got magic 0x{:08x}", sig32);
    }

    delete[] data;
    return ret;
}

int main(int ac, char** av) {
    if (ac < 2) {
        printer::eprintln("Expected filename(s) argument\n{} <FILE...>", av[0]);
        return 1;
    }

    exe_vec results;
    PrintFormat fmt = PrintFormat::TEXT;

    bool process_args = true;

    for (int i = 1; i < ac; i++) {
        if (process_args) {
            auto arg = std::string(av[i]);
            if (arg == "--help" || arg == "-h") {
                printer::println(
                    "{} [OPTIONS] <FILE...>\n\n"
                    "FILE\tThe files(s) to process\n"
                    "Options:\n"
                    "\t--help, -h\tShow this text and exit\n"
                    "\t--json, -j\tOutput JSON data\n"
                    "\t--text, -t\tOutput plaintext data, default\n"
                    "\t--license, -l\tPrint license and copyright\n",
                    av[0]
                );
                return 0;
            }

            if (arg == "--json" || arg == "-j") {
                fmt = PrintFormat::JSON;
                continue;
            }

            if (arg == "--text" || arg == "-t") {
                fmt = PrintFormat::TEXT;
                continue;
            }

            if (arg == "--license" || arg == "-l") {
                printer::print("{}", license);
                return 0;
            }

            if (arg == "--") {
                process_args = false;
                continue;
            }
        }

        auto result = process_image(av[i]);
        if (result.has_value()) { 
            results.append_range(result.value()); 
        } else { 
            return 1; 
        }
    }

    output_main(results, fmt);
    return 0;
}
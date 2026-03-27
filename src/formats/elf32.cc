#include <formats/elf.h>
#include <mains.h>
#include <printr.h>
#include <filesystem>
#include <file.h>

#define S(CP) (std::string((char*)CP))

exefn_result lsbin_elf32main(uchar* data, const char* fname) {
    auto ehdr = (Elf32_Ehdr*)data;
    auto shdrs = (Elf32_Shdr*)&data[ehdr->e_shoff];

    auto shstrtab = &data[shdrs[ehdr->e_shstrndx].sh_offset];
    Elf32_Dyn* dynsect = NULL;
    const uchar* dynstrtab = NULL;
    int ndtags = 0;

    ExecFile file{
        .path = std::filesystem::absolute(fname),
        .type {
            .format = ExecFile::Type::ELF,
            .arch = ExecFile::Type::A32
        },
        .info{}
    };

    for (int i = 0; i < ehdr->e_shnum; i++) {
        Elf32_Shdr this_shdr = shdrs[i];
        if (this_shdr.sh_type == SHT_DYNAMIC) {
            dynsect = (Elf32_Dyn*)&data[this_shdr.sh_offset];
            ndtags = this_shdr.sh_size / sizeof(Elf64_Dyn);
        } else if (this_shdr.sh_type == SHT_PROGBITS &&
                   (S(shstrtab + this_shdr.sh_name) == ".interp")) {
            file.info.interp = (char*)&data[this_shdr.sh_offset];
        } else if (this_shdr.sh_type == SHT_STRTAB &&
                   (S(shstrtab + this_shdr.sh_name) == ".dynstr")) {
            dynstrtab = &data[this_shdr.sh_offset];
        }
    }

    if (dynstrtab == NULL || dynsect == NULL) {
        printer::eprintln("This is not a valid dynamic ELF");
        return std::nullopt;
    }

    for (int i = 0; i < ndtags; i++) {
        if (dynsect[i].d_tag == DT_NEEDED) {
            file.info.libraries.push_back((char*)(dynstrtab + dynsect[i].d_un.d_val));
        }
    }

    return exe_vec{file};
}

#include <elf.h>
#include <printr.h>

#define S(CP) (std::string(CP))

int lsbin_elfmain(char* data) {
    if (data[EI_CLASS] != ELFCLASS64) {
        printer::eprintln("Sorry, this program only works on 64bit files");
        delete[] data;
        return 1;
    }

    auto ehdr = (Elf64_Ehdr*)data;
    auto shdrs = (Elf64_Shdr*)&data[ehdr->e_shoff];

    auto shstrtab = &data[shdrs[ehdr->e_shstrndx].sh_offset];
    Elf64_Dyn* dynsect = NULL;
    const char* dynstrtab = NULL;
    int ndtags = 0;

    for (int i = 0; i < ehdr->e_shnum; i++) {
        Elf64_Shdr this_shdr = shdrs[i];
        if (this_shdr.sh_type == SHT_DYNAMIC) {
            dynsect = (Elf64_Dyn*)&data[this_shdr.sh_offset];
            ndtags = this_shdr.sh_size / sizeof(Elf64_Dyn);
        } else if (this_shdr.sh_type == SHT_PROGBITS &&
                   (S(shstrtab + this_shdr.sh_name) == ".interp")) {
            printer::println("Program interpreter: {}",
                             &data[this_shdr.sh_offset]);
        } else if (this_shdr.sh_type == SHT_STRTAB &&
                   (S(shstrtab + this_shdr.sh_name) == ".dynstr")) {
            dynstrtab = &data[this_shdr.sh_offset];
        }
    }

    if (dynstrtab == NULL || dynsect == NULL) {
        printer::eprintln("This is not a valid dynamic ELF");
        delete[] data;
        return 1;
    }

    for (int i = 0; i < ndtags; i++) {
        if (dynsect[i].d_tag == DT_NEEDED) {
            printer::println("Needed Library: {}",
                             dynstrtab + dynsect[i].d_un.d_val);
        }
    }

    return 0;
}

#include <mag.h>
#include <mains.h>
#include <pe32.h>
#include <printr.h>

int lsbin_pemain(uchar* data) {
    auto nthdrs = (uchar*)&data[*(uint32_t*)&data[0x3c]];

    auto nt = (nthdrs64*)nthdrs;
    if (nt->sig != MAGIC_NT && nt->sig != RMAGIC_NT) {
        printer::eprintln("Invalid NT signature: got 0x{:x}", nt->sig);
        return 1;
    }

    uint32_t importRVA;
    if (nt->opthdr.mag == NT_OPTHDR64_MAG) {
        importRVA = nt->opthdr.datadirs[DIRENT_IMP].vaddr;
    } else {
        auto nt32 = (nthdrs32*)nthdrs;
        importRVA = nt32->opthdr.datadirs[DIRENT_IMP].vaddr;
    }

    if (importRVA == 0) {
        printer::println("No imports");
        return 0;
    }

    auto sects = ((secthdr*)((uintptr_t)(nt) +
                             ((int)__builtin_offsetof(nthdrs64, opthdr)) +
                             ((nt))->nthdr.opthdrsz));

    auto rva_to_offset = [&](uint32_t rva) -> uint32_t {
        for (int i = 0; i < nt->nthdr.numsects; i++) {
            if (rva >= sects[i].vaddr &&
                rva < sects[i].vaddr + sects[i].virtsz) {
                return rva - sects[i].vaddr + sects[i].rdataptr;
            }
        }
        return 0;
    };

    uint32_t importOffset = 0;
    for (int i = 0; i < nt->nthdr.numsects; i++) {
        if (importRVA >= sects[i].vaddr &&
            importRVA < sects[i].vaddr + sects[i].virtsz) {
            importOffset = importRVA - sects[i].vaddr + sects[i].rdataptr;
            break;
        }
    }

    auto imports = (idesc*)(data + importOffset);
    while (imports->namerva != 0) {
        const char* dllName = (char*)(data + rva_to_offset(imports->namerva));
        printer::println("Imported DLL: {}", dllName);
        imports++;
    }

    return 0;
}

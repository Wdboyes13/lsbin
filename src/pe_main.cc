#include <printr.h>
#include <windows.h>

int lsbin_pemain(char* data) {
    auto dos = (IMAGE_DOS_HEADER*)data;

    auto nt = (IMAGE_NT_HEADERS64*)(data + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        printer::eprintln("Invalid NT signature");
        return 1;
    }

    DWORD importRVA;
    if (nt->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        importRVA =
            nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
                .VirtualAddress;
    } else {
        auto nt32 = (IMAGE_NT_HEADERS32*)(data + dos->e_lfanew);
        importRVA =
            nt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
                .VirtualAddress;
    }

    if (importRVA == 0) {
        printer::println("No imports");
        return 0;
    }

    auto sections = IMAGE_FIRST_SECTION(nt);
    DWORD importOffset = 0;
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (importRVA >= sections[i].VirtualAddress &&
            importRVA <
                sections[i].VirtualAddress + sections[i].Misc.VirtualSize) {
            importOffset = importRVA - sections[i].VirtualAddress +
                           sections[i].PointerToRawData;
            break;
        }
    }

    auto rva_to_offset = [&](DWORD rva) -> DWORD {
        for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
            if (rva >= sections[i].VirtualAddress &&
                rva <
                    sections[i].VirtualAddress + sections[i].Misc.VirtualSize) {
                return rva - sections[i].VirtualAddress +
                       sections[i].PointerToRawData;
            }
        }
        return 0;
    };

    auto imports = (IMAGE_IMPORT_DESCRIPTOR*)(data + importOffset);
    while (imports->Name != 0) {
        const char* dllName = data + rva_to_offset(imports->Name);
        printer::println("Imported DLL: {}", dllName);
        imports++;
    }

    return 0;
}

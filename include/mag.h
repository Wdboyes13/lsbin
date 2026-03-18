#pragma once

/*
 * These are the definitions for executable magic bytes, heres how each each
 * name is defined
 *
 * 1. The first part is "MAGIC" or "RMAGIC", MAGIC is the normal
 * magic bytes, and RMAGIC is the byteswapped version
 *
 * 2. The second part is the executable type:
 * - 1. MF - Mach-O FAT
 * - 2. MH - Mach-O Normal
 * - 3. ELF - ELF
 * - 4. DOS - PE DOS Header
 * - 5. NT - PE NT Header
 *
 * 3. An optional part is the word size, this is either 32 or 64
 */

#include <cstdint>

#define MAGIC_MF32 0xcafebabe
#define RMAGIC_MF32 0xbebafeca

#define MAGIC_MF64 0xcafebabf
#define RMAGIC_MF64 0xbfbafeca

#define MAGIC_MH64 0xfeedfacf
#define RMAGIC_MH64 0xcffaedfe

#define MAGIC_MH32 0xfeedface
#define RMAGIC_MH32 0xcefaedfe

#define MAGIC_ELF (0x7f << 24 | 'E' << 16 | 'L' << 8 | 'F')
#define RMAGIC_ELF ('F' << 24 | 'L' << 16 | 'E' << 8 | 0x7f)

#define MAGIC_DOS ('M' << 8 | 'Z')
#define RMAGIC_DOS ('Z' << 8 | 'M')

#define MAGIC_NT ('P' << 24 | 'E' << 16 | 0x0 << 8 | 0x0)
#define RMAGIC_NT (0x0 << 24 | 0x0 << 16 | 'E' << 8 | 'P')

template<typename T>
inline T swap_endian(T value) {
    static_assert(sizeof(T) <= 8, "Type too large");

    union {
        T value;
        uint8_t bytes[sizeof(T)];
    } src, dest;

    src.value = value;

    for (size_t i = 0; i < sizeof(T); i++) {
        dest.bytes[i] = src.bytes[sizeof(T) - 1 - i];
    }

    return dest.value;
}

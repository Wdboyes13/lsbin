#pragma once

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

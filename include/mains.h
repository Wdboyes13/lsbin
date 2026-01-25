#pragma once

#include <cstdint>

typedef uint8_t uchar;

int lsbin_elf64main(uchar* data);
int lsbin_elf32main(uchar* data);
int lsbin_machomain(uchar* data);
int lsbin_pemain(uchar* data);

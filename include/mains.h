#pragma once

#include <cstdint>
#include <file.h>
#include <optional>

typedef uint8_t uchar;

typedef std::vector<ExecFile> exe_vec;
typedef std::optional<exe_vec> exefn_result;

exefn_result lsbin_elf64main(uchar* data, const char* fname);
exefn_result  lsbin_elf32main(uchar* data, const char* fname);
exefn_result  lsbin_machomain(uchar* data, const char* fname); // this returns a vec because of fat machos
exefn_result  lsbin_pemain(uchar* data, const char* fname);

enum class PrintFormat {
    JSON,
    TEXT  
};

void output(exe_vec res, PrintFormat fmt);
exefn_result process_image(const char* fname);
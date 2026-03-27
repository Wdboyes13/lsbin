#pragma once

#include <cstdint>

// sources:
// - https://en.wikipedia.org/wiki/Mach-O
// - https://developer.apple.com/documentation/kernel/mach-o
// - https://developer.apple.com/documentation/kernel/lc_str
// - https://developer.apple.com/documentation/kernel/load_command
// - https://github.com/aidansteele/osx-abi-macho-file-format-reference

typedef struct {
    uint32_t cmd;
    uint32_t cmdsize;
} load_command;

union lc_str {
    uint32_t offset;
};

typedef int32_t cpu_subtype_t;

#define LC_LOAD_DYLIB 0xc
#define LC_LOAD_DYLINKER 0xe

struct dylib {
    union lc_str name;
    uint32_t timestamp;
    uint32_t current_version;
    uint32_t compatibility_version;
};

struct dylib_command {
    uint32_t cmd;
    uint32_t cmdsize;
    struct dylib dylib;
};

struct dylinker_command {
    uint32_t cmd;
    uint32_t cmdsize;
    union lc_str name;
};

struct fat_header {
    uint32_t magic;
    uint32_t nfat_arch;
};

struct fat_arch {
    int32_t cputype;
    int32_t cpusubtype;
    uint32_t offset;
    uint32_t size;
    uint32_t align;
};

struct fat_arch_64 {
    int32_t cputype;
    int32_t cpusubtype;
    uint64_t offset;
    uint64_t size;
    uint32_t align;
    uint32_t reserved;
};

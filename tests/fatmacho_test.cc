#include <test_lib.h>
#include <format>

int main() {
    auto out = run_lsbin_test("a.fat_macho");
    return (out == std::format("Mach-O File: {}\n"
                               "FAT Index: 1\n"
                               "Load dylinker: /usr/lib/dyld\n"
                               "Load dylib: /usr/lib/libc++.1.dylib\n"
                               "Load dylib: /usr/lib/libSystem.B.dylib\n"
                               "FAT Index: 2\n"
                               "Load dylinker: /usr/lib/dyld\n"
                               "Load dylib: /usr/lib/libc++.1.dylib\n"
                               "Load dylib: /usr/lib/libSystem.B.dylib\n",
                               test_file_path("a.fat_macho")))
               ? 0
               : 1;
}

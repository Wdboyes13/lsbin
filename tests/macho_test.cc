#include <test_lib.h>
#include <format>

int main() {
    auto out = run_lsbin_test("a.macho");
    return (out == std::format("Mach-O File: {}\n"
                               "Load dylinker: /usr/lib/dyld\n"
                               "Load dylib: /usr/lib/libc++.1.dylib\n"
                               "Load dylib: /usr/lib/libSystem.B.dylib\n",
                               test_file_path("a.macho")))
               ? 0
               : 1;
}

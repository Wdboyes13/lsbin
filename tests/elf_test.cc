#include <test_lib.h>
#include <format>

int main() {
    auto out = run_lsbin_test("a.elf");
    return (out == std::format("ELF File: {}\n"
                               "Program interpreter: /lib/ld-linux-aarch64.so.1\n"
                               "Needed Library: libstdc++.so.6\n"
                               "Needed Library: libm.so.6\n"
                               "Needed Library: libgcc_s.so.1\n"
                               "Needed Library: libc.so.6\n",
                               test_file_path("a.elf")))
               ? 0
               : 1;
}

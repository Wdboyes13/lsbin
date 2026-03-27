#include <test_lib.h>

int main() {
    auto out_opt = run_lsbin_test("a.elf");
    if (!out_opt) { return 1; }
    auto out_vec = out_opt.value();
    if (out_vec.size() != 1) { return 1; }
    auto out = out_vec.at(0);

    if (out.type.arch == Arch::A64 &&
        out.type.format == Fmt::ELF &&
        out.info.interp == "/lib/ld-linux-aarch64.so.1" &&
        out.info.libraries.size() == 4 &&
        has_lib(out, "libstdc++.so.6") &&
        has_lib(out, "libm.so.6") &&
        has_lib(out, "libgcc_s.so.1") &&
        has_lib(out, "libc.so.6")
    ) {
        return 0;
    }
    
    return 1;
}

#include <test_lib.h>

bool is_arch_ok(const ExecFile& out) {
    return (out.type.arch == Arch::A64 &&
        out.type.format == Fmt::FAT_MACH_O &&
        out.info.interp == "/usr/lib/dyld" &&
        out.info.libraries.size() == 2 &&
        has_lib(out, "/usr/lib/libc++.1.dylib") &&
        has_lib(out, "/usr/lib/libSystem.B.dylib"));
}

int main() {
    auto out_opt = run_lsbin_test("a.fat_macho");
    if (!out_opt) { return 1; }
    auto out_vec = out_opt.value();
    if (out_vec.size() != 2) { return 1; }
    
    if (is_arch_ok(out_vec.at(0)) && is_arch_ok(out_vec.at(1))) {
        return 0;
    } else {
        return 1;
    }
}

#include <test_lib.h>

int main() {
    auto out_opt = run_lsbin_test("a.macho");
    if (!out_opt) { return 1; }
    auto out_vec = out_opt.value();
    if (out_vec.size() != 1) { 
        return 1; 
    }
    auto out = out_vec.at(0);

    if (out.type.arch == Arch::A64 &&
        out.type.format == Fmt::MACH_O &&
        out.info.interp == "/usr/lib/dyld" &&
        out.info.libraries.size() == 2 &&
        has_lib(out, "/usr/lib/libc++.1.dylib") &&
        has_lib(out, "/usr/lib/libSystem.B.dylib")
    ) {
        return 0;
    }
    return 1;
}

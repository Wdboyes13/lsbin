#include <test_lib.h>

int main() {
    auto out_opt = run_lsbin_test("a.exe");
    if (!out_opt) { return 1; }
    auto out_vec = out_opt.value();
    if (out_vec.size() != 1) { return 1; }
    auto out = out_vec.at(0);

    if (out.type.arch == Arch::A64 &&
        out.type.format == Fmt::PE &&
        out.info.interp.empty() &&
        out.info.libraries.size() == 1 &&
        has_lib(out, "KERNEL32.dll")) {
        return 0;
    }

    return 1;
}

#include <test_lib.h>
#include <format>

int main() {
    auto out = run_lsbin_test("a.exe");
    return (out == std::format("PE File: {}\n"
                               "Imported DLL: KERNEL32.dll\n",
                               test_file_path("a.exe")))
               ? 0
               : 1;
}

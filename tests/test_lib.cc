#include "test_lib.h"
#include <test_config.h>

std::string run_lsbin_test(const fs::path& file) {
    std::string cmd = std::string(LSBIN_PATH) + " " + test_file_path(file);
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return "";
    }

    std::string result;
    char buf[128];

    while (fgets(buf, sizeof(buf), pipe)) {
        result += buf;
    }

    pclose(pipe);
    return result;
}

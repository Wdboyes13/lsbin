#include <test_config.h>
#include <test_lib.h>
#include <cstdlib>

std::optional<std::vector<ExecFile>> run_lsbin_test(const fs::path& file) {
    std::string cmd = std::string(LSBIN_PATH) + " --json " + test_file_path(file);
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return std::nullopt;
    }

    std::string result;
    char buf[128];

    while (fgets(buf, sizeof(buf), pipe)) {
        result += buf;
    }

    pclose(pipe);
    return json::parse(result).get<std::vector<ExecFile>>();
}

bool has_lib(const ExecFile& file, const std::string& s) {
    auto& libs = file.info.libraries;
    return std::find(libs.begin(), libs.end(), s) != libs.end();
}
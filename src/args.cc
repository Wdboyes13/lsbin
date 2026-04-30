#include <args.h>

Args& Args::argc(int argc) {
    this->arg_c = argc;
    return *this;
}

Args& Args::argv(char** argv) {
    this->arg_v = argv;
    return *this;
}

Args& Args::parse() {
    if (arg_c == 0 || arg_v == NULL) {
        return *this;
    }

    bool process_args = true;
    for (int i = 1; i < arg_c; i++) {
        if (process_args) {
            auto arg = std::string(arg_v[i]);
            if (arg == "--help" || arg == "-h") {
                do_help();
            }

            if (arg == "--json" || arg == "-j") {
                fmt = PrintFormat::JSON;
                continue;
            }

            if (arg == "--text" || arg == "-t") {
                fmt = PrintFormat::TEXT;
                continue;
            }

            if (arg == "--license" || arg == "-l") {
                do_license();
            }

            if (arg == "--github" || arg == "-g") {
                printer::println("GitHub: https://github.com/Wdboyes13/lsbin");
                exit(0);
            }

            if (arg == "--") {
                process_args = false;
                continue;
            }
        }

        files.push_back(arg_v[i]);
    }

    return *this;
}

[[noreturn]] void Args::do_help() {
    printer::println(
        "{} [OPTIONS] <FILE...>\n\n"
        "FILE    The files(s) to process\n"
        "Options:\n"
        "    --help, -h       Show this text and exit\n"
        "    --json, -j       Output JSON data\n"
        "    --text, -t       Output plaintext data, default\n"
        "    --license, -l    Print license and copyright and exits\n"
        "    --github, -g     Prints the GitHub link and exits",
        arg_v[0]);
    exit(0);
}

[[noreturn]] void Args::do_license() {
    printer::print("{}", license);
    exit(0);
}
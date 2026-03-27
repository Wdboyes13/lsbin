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
        "FILE\tThe files(s) to process\n"
        "Options:\n"
        "\t--help, -h\tShow this text and exit\n"
        "\t--json, -j\tOutput JSON data\n"
        "\t--text, -t\tOutput plaintext data, default\n"
        "\t--license, -l\tPrint license and copyright\n",
        arg_v[0]);
    exit(0);
}

[[noreturn]] void Args::do_license() {
    printer::print("{}", license);
    exit(0);
}
#include <args.h>
#include <mains.h>
#include <printr.h>

int main(int ac, char** av) {
    exe_vec results;

    Args args = Args()
                    .argc(ac)
                    .argv(av)
                    .parse();

    for (const auto& file : args.files) {
        auto result = process_image(file.c_str());
        if (!result) {
            return 1;
        }
        results.append_range(result.value());
    }

    output(results, args.fmt);
    return 0;
}
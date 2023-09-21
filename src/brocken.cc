#include "config.hh"
#include "rocken.hh"

#include <CLI/CLI.hpp>
#include <fmt/color.h>
#include <fmt/core.h>

#include <iostream>
#include <string>

auto main(int argc, char **argv) -> int {
    CLI::App app{fmt::format("Brocken {} ({}:{}) on {}", BROCKEN_VERSION, GIT_BRANCH, GIT_COMMIT,
                             _BROCKEN_PLAT)};
    app.get_formatter()->column_width(40);

    bool repl_flag{false};
    std::string_view file;
    app.add_option("programfile", file, "programfile")->check(CLI::ExistingFile);
    std::vector<std::string_view> exec;
    app.add_option("-e", exec, "one line of program (several -e's allowed, omit programfile");

    CLI11_PARSE(app, argc, argv);

    if (!file.size()) {
        fmt::print(fmt::emphasis::bold, "Brocken");
        fmt::print("Type \"#help\" for more information\n");
        std::string in;
        bool repl_flag = true;

        while (repl_flag) {
            fmt::print("{}>>> ", "" /* single icon indicating state */);
            std::getline(std::cin, in);
            // Remove leading/trailing whitespaces
            if (!in.compare("#quit")) { repl_flag = false; }
            else if (!in.compare("#help")) {
                fmt::print("To use this interactive REPL, just type in regular code and hit\n");
                fmt::print("enter. You can also make use of the following commands:\n\n");
                //~ fmt::print("Loads variable and function declarations from a specified\n");
                //~ fmt::print("					file into memory, e.g.\n");
                //~ fmt::print("    >>> #load ~/hello_world.prog\n\n");
                fmt::print(" #help				Display this text\n\n");
                fmt::print(" #quit				Exits the REPL\n\n");
                //~ fmt::print(" #st				Displays the symbol table, a list of
                //currently declared \n");
                //~ fmt::print("					functions and variables in the global
                //scope.\n\n"); ~ fmt::print(" #clear				Clears the terminal
                //window.\n\n");
            }
        }
    }
    else {
        // TODO: handle other params and potentially load a file
    }
    return 0;
}

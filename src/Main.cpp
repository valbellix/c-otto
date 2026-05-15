#include <iostream>
#include <string>
#include <CLI/CLI.hpp>

#include "c8/Emulator.h"

int main(int argc, char* argv[]) {
    CLI::App cliApp {"Chip-8 emulator/interpreter"};
    std::string fileName;
    bool debugFlag = false;
    cliApp.add_option("-f,--file", fileName, "File to run");
    cliApp.add_flag("-d,--debug", debugFlag, "Enable debug mode");

    CLI11_PARSE(cliApp, argc, argv);

    Emulator emu(1024, 512, debugFlag);
    try {
        emu.init();
        emu.loadFile(fileName);
        emu.start();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

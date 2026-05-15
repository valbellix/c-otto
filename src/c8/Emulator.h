#ifndef _Emulator_h_
#define _Emulator_h_

#include "Cpu.h"
#include "System.h"

#include <string>
#include <memory>

class Emulator {
public:
    Emulator(int width, int height, bool debugFlag);
    ~Emulator() {}

    void init();
    void loadFile(const std::string& fileName);
    void start();

private:
    Cpu m_cpu;
    std::unique_ptr<System> m_system;
    static constexpr std::string TITLE = "chip-8 emulator";
};

#endif
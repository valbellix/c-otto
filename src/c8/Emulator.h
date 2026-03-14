#ifndef _Emulator_h_
#define _Emulator_h_

#include "Cpu.h"

#include <string>

class Emulator {
public:
    Emulator() {}
    ~Emulator() {}

    void init();
    void loadFile(const std::string& fileName);

private:
    Cpu m_cpu;
};

#endif
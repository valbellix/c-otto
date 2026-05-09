#include "Emulator.h"
#include "exceptions/FileNotFoundException.h"
#include "exceptions/OutOfBoundaryException.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <thread>

#include "SDLSystem.h"

inline bool fileExist(const std::string& fileName) {
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

Emulator::Emulator(const int width, const int height)
        : m_cpu(), m_system(new SDLSystem(Emulator::TITLE, width, height)) {
}

void Emulator::init() {
    m_cpu.init();
}

void Emulator::start() {
    do {
        m_cpu.executeCycle();
        if (m_cpu.updateScreen()) {
            m_cpu.setUpdateScreen(false);
            m_system->update(m_cpu.getGraphicBuffer());
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    } while (true);
}

void Emulator::loadFile(const std::string& fileName) {
    if (!fileExist(fileName)) {
        throw FileNotFoundException(fileName);
    }

    std::uintmax_t size = std::filesystem::file_size(fileName);
    if (size > (0xFFF - 0x200)) {
        throw OutOfBoundaryException("The file cannot be load as it exceeds the available memory");
    }
    char buffer[size];
    std::ifstream fin(fileName, std::ios::binary);
    try {
        fin.read(buffer, size);
        m_cpu.loadBufferIntoMemory(reinterpret_cast<uint8_t *>(buffer), size);
    } catch (...) {
        fin.close();
        throw;
    }
    fin.close();
}
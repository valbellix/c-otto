#include "Emulator.h"
#include "exceptions/FileNotFoundException.h"
#include "exceptions/OutOfBoundaryException.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

inline bool fileExist(const std::string& fileName) {
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

void Emulator::init() {
    m_cpu.init();
}

void Emulator::loadFile(const std::string& fileName) {
    if (!fileExist(fileName)) {
        throw new FileNotFoundException(fileName);
    }

    std::uintmax_t size = std::filesystem::file_size(fileName);
    if (size > (0xFFF - 0x200)) {
        throw new OutOfBoundaryException("The file cannot be load as it exceeds the available memory");
    }
    uchar buffer[size];
    std::ifstream fin(fileName, std::ios::binary);
    try {
        fin.read((char*) buffer, size);
        m_cpu.loadBufferIntoMemory(buffer, size);
    } catch (...) {
        fin.close();
        throw;
    }
    fin.close();
}
#include "Cpu.h"
#include "exceptions/OutOfBoundaryException.h"
#include "exceptions/BufferNotValidException.h"

#include <cstring>
#include <string>


Cpu::Cpu() {}

void Cpu::init() {
    m_index = 0;
    // the system expect the application to be loaded starting at 0x200 that would be the PC starting value
    m_pc = 0x200;
    m_delayTimer = 0;
    m_soundTimer = 0;
    m_stackPointer = 0;

    // clear memory and load fontset from 0x50
    memset(m_memory, 0, sizeof(m_memory));
    loadFontSet(0x50);

    memset(m_registerV, 0, sizeof(m_registerV));
    memset(m_graphicSys, 0, sizeof(m_graphicSys));
    memset(m_stack, 0, sizeof(m_stack));
    memset(m_key, 0, sizeof(m_key));


}

void Cpu::loadBufferIntoMemory(const uchar* buffer, const size_t length) {
    if ((0x200 + length) > 0xFFF) {
        throw OutOfBoundaryException("The buffer is too big to be loaded");
    } else if (buffer == NULL || length == 0) {
        throw BufferNotValidException();
    }
    for (ushort i = 0x200; i < 0xFFF; ++i) {
        m_memory[i] = buffer[i];
    }
}

void Cpu::loadFontSet(const ushort startLocation) {
    for (ushort i = startLocation; i < 80; ++i) {
        m_memory[i] = m_fontSet[i];
    }
}

void Cpu::executeCycle() {
    // fetch opcode
    // decode opcode
    // execute opcode
    // update timers
}

void Cpu::fetchOpCode() {
}

const uchar Cpu::m_fontSet[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
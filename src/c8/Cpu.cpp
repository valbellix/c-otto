#include "Cpu.h"
#include "exceptions/OutOfBoundaryException.h"
#include "exceptions/BufferNotValidException.h"
#include "exceptions/UnkownOpCodeException.h"

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
    ushort opCode = fetchOpCode();
    executeOpCode(opCode);

    // update timers
    if (m_delayTimer > 0) {
        --m_delayTimer;
    }
    if (m_soundTimer > 0) {
        if (m_soundTimer == 1) {
            beep();
        }
        m_soundTimer--;
    }
}

ushort Cpu::fetchOpCode() const {
    // each address contains 1 byte, opcodes are 2 bytes long
    // it means that we need to merge two successive occurrences
    // in a ushort (2 bytes) so we shift the first 8 bits to the right 
    // and then bitwise OR with the second occurrence
    return m_memory[m_pc] << 8 | m_memory[m_pc+1];
}

void Cpu::beep() {
    std::cout << "BEEP!" << std::endl;
}

void Cpu::executeOpCode(ushort opCode) {
    // bitmasks to be used to decode the opcode
    const ushort firstMask = 0xF000;
    const ushort secondMask = 0x0F00;
    const ushort lastTwoMask = 0x00FF;

    const ushort opCodeClass = opCode & firstMask;
    const ushort secondChar = (opCode & secondMask) >> 8;
    const uchar lastTwo = opCode & lastTwoMask;

    switch (opCodeClass)
    {
    case 0x0000:
        break;
    case 0x1000:
        break;
    case 0x2000:
        break;
    case 0x3000:
        break;
    case 0x4000:
        break;
    case 0x5000:
        break;
    case 0x6000:
        // 6XNN - sets VX to NN
        m_registerV[secondChar] = lastTwo;
        break;
    case 0x7000:
        // 7XNN - adds NN to VX
        m_registerV[secondChar] += lastTwo;
        break;
    case 0x8000:
        break;
    case 0x9000:
        break;
    case 0xA000:
        break;
    case 0xB000:
        break;
    case 0xC000:
        break;
    case 0xD000:
        break;
    case 0xE000:
        break;
    case 0xF000:
        break;
    default:
        throw new UnknownOpCodeException(opCode);
    }

    // then increment the PC by two
    m_pc += 2;
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
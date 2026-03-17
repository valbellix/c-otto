#include "Cpu.h"
#include "exceptions/OutOfBoundaryException.h"
#include "exceptions/BufferNotValidException.h"
#include "exceptions/UnkownOpCodeException.h"

#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>

Cpu::Cpu(const Type type): m_type(type), m_updateScreen(false) {}

Cpu::Cpu(): Cpu(COSMAC_VIP) {}

void Cpu::init() {
    m_index = 0;
    // the system expect the application to be loaded starting at 0x200 that would be the PC starting value
    m_pc = 0x200;
    m_delayTimer = 0;
    m_soundTimer = 0;
    m_stackPointer = 0;

    m_updateScreen = false;

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
    const ushort lastThreeMask = 0x0FFF;
    const ushort lastMask = 0x000F;

    const ushort opCodeClass = opCode & firstMask;
    const ushort secondNibble = (opCode & secondMask) >> 8;
    const uchar lastTwoNibbles = opCode & lastTwoMask;
    const ushort lastThreeNibbles = opCode & lastThreeMask;
    const uchar thirdNibble = (lastThreeNibbles & 0x100) >> 8;
    const uchar lastNibble = static_cast<const uchar>(opCode & lastMask);

    bool incrementPc = true;

    short subCode;
    uchar originalValue;

    switch (opCodeClass) {
    case 0x0000:
        switch (opCode) {
        case 0x00E0:
            // 00E0 - clears the screen
            memset(m_graphicSys, 0, sizeof(m_graphicSys));
            break;
        case 0x00EE:
            // 00EE - jumps back form subroutine
            m_pc = m_stack[m_stackPointer--];
            break;
        default:
            throw new UnknownOpCodeException(opCode);
        }
        break;
    case 0x1000:
        // 1NNN - jumps to NNN
        m_pc = lastThreeNibbles;
        incrementPc = false;
        break;
    case 0x2000:
        // 2NNN - calls subroutine at address NNN (it stores the current pc to the stack)

        // TODO: what if the stack is full?
        m_stack[m_stackPointer++] = m_pc;
        m_pc = lastThreeNibbles;
        break;
    case 0x3000:
        // 3XNN - skips one instruction if V[X] == NN
        if (m_registerV[secondNibble] == lastTwoNibbles) {
            m_pc += 2;
        }
        break;
    case 0x4000:
        // 4XNN - skips one instruction if V[X] != NN
        if (m_registerV[secondNibble] != lastTwoNibbles) {
            m_pc += 2;
        }
        break;
    case 0x5000:
        // 5XY0 - skips one instruction if V[X] == V[Y]
        if (m_registerV[secondNibble] == m_registerV[thirdNibble]) {
            m_pc += 2;
        }
        break;
    case 0x6000:
        // 6XNN - sets VX to NN
        m_registerV[secondNibble] = lastTwoNibbles;
        break;
    case 0x7000:
        // 7XNN - adds NN to VX
        m_registerV[secondNibble] += lastTwoNibbles;
        // TODO: any carry flag to set???
        break;
    case 0x8000:
        subCode = opCode & 0xF00F;
        switch (subCode) {
        case 0x8000:
            // 8XY0 - sets VX to VY
            m_registerV[secondNibble] = m_registerV[thirdNibble];
            break;
        case 0x8001:
            // 8XY1 - sets VX to VX | VY
            m_registerV[secondNibble] = m_registerV[secondNibble] | m_registerV[thirdNibble];
            break;
        case 0x8002:
            // 8XY2 - sets VX to VX & VY
            m_registerV[secondNibble] = m_registerV[secondNibble] & m_registerV[thirdNibble];
            break;
        case 0x8003:
            // 8XY3 - sets VX to VX ^ VY
            m_registerV[secondNibble] = m_registerV[secondNibble] ^ m_registerV[thirdNibble];
            break;
        case 0x8004:
            // 8XY4 - sets VX to VX + VY and sets VF=1 if the result overflows
            originalValue = m_registerV[secondNibble];
            m_registerV[secondNibble] = m_registerV[secondNibble] + m_registerV[thirdNibble];
            m_registerV[0xF] = (m_registerV[secondNibble] < originalValue) ? 1 : 0;
            break;
        case 0x8005:
            // 8XY5 - sets VX to VX - VY and sets VF=1, if we borrow from VF, we set it to 0
            originalValue = m_registerV[secondNibble];
            m_registerV[secondNibble] = m_registerV[secondNibble] - m_registerV[thirdNibble];
            m_registerV[0xF] = (originalValue >= m_registerV[thirdNibble]) ? 1 : 0;
            break;
        case 0x8006:
            // 8XY6 - sets VX to VY if COSMAC_VIP and shift left, it sets VF to the bit shifted out.
            //        Otherwise it ignores VY and just shift VX
            if (m_type == COSMAC_VIP) {
                m_registerV[secondNibble] = m_registerV[thirdNibble];
            }
            m_registerV[0xF] = ((m_registerV[secondNibble] & 0x80) == 0x80) ? 1 : 0;
            m_registerV[secondNibble] = m_registerV[secondNibble] << 1;
        case 0x8007:
            // 8XY7 - like 8XY5, but we do VY - VX
            originalValue = m_registerV[secondNibble];
            m_registerV[secondNibble] = m_registerV[thirdNibble] - m_registerV[secondNibble];
            m_registerV[0xF] = (m_registerV[thirdNibble] >= originalValue) ? 1 : 0;
            break;
        case 0x800E:
            // 8XYE - sets VX to VY if COSMAC_VIP and shift right, it sets VF to the bit shifted out.
            //        Otherwise it ignores VY and just shift VX
            if (m_type == COSMAC_VIP) {
                m_registerV[secondNibble] = m_registerV[thirdNibble];
            }
            m_registerV[0xF] = ((m_registerV[secondNibble] & 0x80) == 0x80) ? 1 : 0;
            m_registerV[secondNibble] = m_registerV[secondNibble] >> 1;
        default:
            throw new UnknownOpCodeException(opCode);
        }
        break;
    case 0x9000:
        // 9XY0 - skips one instruction if V[X] == V[Y]
        if (m_registerV[secondNibble] != m_registerV[thirdNibble]) {
            m_pc += 2;
        }
        break;
    case 0xA000:
        // ANNN - sets index I to NNN
        m_index = lastThreeNibbles;
        break;
    case 0xB000:
        // BNNN - jumps to NNN + the value of V0 if the type is COSMAC_VIP, otherwise
        //        it decodes as BXNN and it jums to NNN + VX
        if (m_type == COSMAC_VIP) {
            m_pc = lastThreeNibbles + m_registerV[0];
        } else {
            m_pc = lastTwoNibbles + m_registerV[thirdNibble];
        }
        incrementPc = false;
        break;
    case 0xC000:
        // CXNN - generates a random number, it does a bitwise AND with NN and store in VX
        std::srand(std::time(0));
        m_registerV[secondNibble] = (static_cast<uchar>(std::rand()) % UCHAR_MAX) & lastTwoNibbles;
        break;
    case 0xD000:
        // DXYN - well... it displays a sprite of N pixels starting from I to XY
        //        the sprite should not go over 64x32, so it should be cropped
        display(m_registerV[secondNibble] & 63, m_registerV[thirdNibble] & 31, lastNibble);
        break;
    case 0xE000:
        switch (lastTwoNibbles) {
        case 0x9E:
            // EX9E - skip the following instruction if the key pointed by VX != 0 (i.e. is pressed)
            if (m_key[m_registerV[secondNibble]] != 0) {
                m_pc += 4;
                incrementPc = false;
            }
            break;
        case 0xA1:
            // EXA1 - skip the following instruction if the key pointed by VX == 0 (i.e. is not pressed)
            if (m_key[m_registerV[secondNibble]] == 0) {
                m_pc += 4;
                incrementPc = false;
            }
            break;
        default:
            throw new UnknownOpCodeException(opCode);
        }
        break;
    case 0xF000:
        switch (lastTwoNibbles) {
        case 0x07:
            // FX07 - sets VX to the delay timer value
            m_registerV[secondNibble] = m_delayTimer;
            break;
        case 0x15:
            // FX15 - sets the delay timer to the value in VX
            m_delayTimer = m_registerV[secondNibble];
            break;
        case 0x18:
            // FX18 sets the sound timer to the value in VX
            m_soundTimer = m_registerV[secondNibble];
            break;
        default:
            throw new UnknownOpCodeException(opCode);
        }
        break;
    default:
        throw new UnknownOpCodeException(opCode);
    }

    if (incrementPc) {
        // then increment the PC by two
        m_pc += 2;
    }
}

void Cpu::display(const uchar startX, const uchar startY, const uchar height) {
    m_registerV[0xF] = 0;
    for (uchar y = 0; y < height; ++y) {
        uchar row = m_memory[m_index + y];
        for (uchar x = 0; x < 8; ++x) {
            if ((row & (0x80 >> x)) != 0) {
                ushort index = startX + x + ((startY + y) * 64);
                if (m_graphicSys[index] == 1) {
                    m_registerV[0xF] = 1;
                }
                m_graphicSys[index] ^= 1;
            }
        }
    }

    m_updateScreen = true;
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
#ifndef _Cpu_h_
#define _Cpu_h_

#include "Defs.h"
#include <cstdlib>

#define MEM_SIZE 4096
#define REG_SIZE 16
#define PIXELS 2048
#define STACK_LEVELS 16
#define KEYPAD_STATES 16

class Cpu {
public:
    enum Type {
        COSMAC_VIP,
        CHIP_48,
        SUPER_CHIP
    };

    Cpu();
    Cpu(const Type);
    ~Cpu() {}

    void init();
    void executeCycle();
    void loadBufferIntoMemory(const uchar* buffer, const size_t lenght);

    Type getType() const { return m_type; }

private:
    uchar m_memory[MEM_SIZE];

    uchar m_registerV[REG_SIZE];
    ushort m_index;
    ushort m_pc;

    uchar m_graphicSys[PIXELS];

    uchar m_delayTimer;
    uchar m_soundTimer;

    ushort m_stack[STACK_LEVELS];
    ushort m_stackPointer;

    uchar m_key[KEYPAD_STATES];

    Type m_type;

    ushort fetchOpCode() const;
    void executeOpCode(ushort opCode);
    void loadFontSet(const ushort startLocation);
    void beep();

    static const uchar m_fontSet[];
};

#endif

#ifndef _Cpu_h_
#define _Cpu_h_

#include <cstdint>
#include <cstdlib>

#define MEM_SIZE 4096
#define REG_SIZE 16
#define PIXELS 2048
#define PIXEL_ON 0xFFFFFFFF
#define PIXEL_OFF 0x00000000
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
    void loadBufferIntoMemory(const uint8_t* buffer, const size_t length);

    bool updateScreen() const { return m_updateScreen; }

    Type getType() const { return m_type; }

    const uint32_t* getGraphicBuffer() const { return m_graphicSys; }

    void setUpdateScreen(const bool updateScreen) { m_updateScreen = updateScreen; }

private:
    uint8_t m_memory[MEM_SIZE]{};

    uint8_t m_registerV[REG_SIZE]{};
    uint16_t m_index;
    uint16_t m_pc;

    uint32_t m_graphicSys[PIXELS]{};

    uint8_t m_delayTimer;
    uint8_t m_soundTimer;

    uint16_t m_stack[STACK_LEVELS]{};
    uint16_t m_stackPointer;

    uint8_t m_key[KEYPAD_STATES]{};

    Type m_type;

    bool m_updateScreen;

    uint16_t fetchOpCode() const;
    void executeOpCode(uint16_t opCode);
    void loadFontSet();
    void beep();

    void display(const uint8_t x, const uint8_t y, const uint8_t n);

    static const uint8_t m_fontSet[];
    static const uint16_t m_fontStartLocation = 0x50;
};

#endif

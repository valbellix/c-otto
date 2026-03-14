#ifndef _UnknownOpCodeException_h_
#define _UnknownOpCodeException_h_

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../Defs.h"

class UnknownOpCodeException: public std::exception {
public:
    UnknownOpCodeException(const ushort opCode) {
        std::stringstream ss;
        ss << "0x" << std::hex << std::uppercase << opCode;
        m_message = "OpCode '" + ss.str() + "' is unknown";
    }

    virtual ~UnknownOpCodeException() {}
    virtual const char* what() const noexcept {
        return m_message.c_str();
    }
private:
    std::string m_message;
};

#endif
#ifndef _InvalidKeyPadPositionException_h_
#define _InvalidKeyPadPositionException_h_

#include <exception>
#include <format>

class InvalidKeyPadPositionException : public std::exception {
public:
    InvalidKeyPadPositionException(size_t position): m_position(position) {}

    virtual const char* what() const throw() {
        return std::format("Invalid key pad position: %d", m_position).c_str();
    }
private:
    size_t m_position;
};

#endif

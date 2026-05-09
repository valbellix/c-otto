#ifndef _System_h
#define _System_h

#include <string>
#include <cstdint>

class System {
public:
    System(const std::string& title, const int width, const int height)
        : m_title(title), m_width(width), m_height(height) {}
    virtual ~System() {}
    virtual bool keyEvent(const uint8_t) = 0;
    virtual void update(const void* buffer) = 0;
protected:
    std::string m_title;
    int m_width;
    int m_height;
};

#endif
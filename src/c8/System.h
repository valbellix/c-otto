#ifndef _System_h
#define _System_h

#include <string>

#include "Defs.h"

class System {
public:
    System(const std::string& title, const int scale, const int height, const int width): m_title(title), m_scale(scale), m_height(height), m_width(width) {}
    virtual ~System() {}
    virtual bool keyEvent(const uchar) = 0;
    virtual void update(const void* buffer) = 0;
protected:
    std::string m_title;
    int m_scale;
    int m_height;
    int m_width;
};

#endif
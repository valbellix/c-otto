#ifndef _VideoException_h
#define _VideoException_h

#include <stdexcept>

class VideoException: public std::runtime_error {
public:
    VideoException(const std::string& msg): std::runtime_error(msg) {}
};

#endif
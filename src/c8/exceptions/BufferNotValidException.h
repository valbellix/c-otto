#ifndef _BufferNotValidException_h_
#define _BufferNotValidException_h_

#include <stdexcept>

class BufferNotValidException: public std::runtime_error {
public:
    BufferNotValidException(): std::runtime_error("The buffer is not valid") {}
};

#endif
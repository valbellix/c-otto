#ifndef _OutOfBoundaryException_h_
#define _OutOfBoundaryException_h_

#include <stdexcept>

class OutOfBoundaryException: public std::runtime_error {
public:
    OutOfBoundaryException(const std::string& msg): std::runtime_error(msg) {}
};

#endif

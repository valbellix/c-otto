#ifndef _FileNotFoundException_h_
#define _FileNotFoundException_h_

#include <stdexcept>
#include <format>

class FileNotFoundException: public std::runtime_error {
public:
    FileNotFoundException(const std::string& fileName): 
        std::runtime_error(std::format("{} not found", fileName)) {}
};

#endif
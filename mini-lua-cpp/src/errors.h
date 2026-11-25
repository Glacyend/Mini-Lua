#ifndef __ERRORS_H__
#define __ERRORS_H__

#include "location.h"

#include <stdexcept>
#include <string>

class LexicalError : public std::runtime_error {
public:
    std::string message;
    Location location;

    LexicalError(std::string message, Location location);
};

class SyntaxError : public std::runtime_error {
public:
    std::string message;
    Location location;

    SyntaxError(std::string message, Location location);
};

#endif

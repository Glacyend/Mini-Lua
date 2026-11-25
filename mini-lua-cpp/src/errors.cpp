#include "errors.h"

#include <format>
#include <utility>

LexicalError::LexicalError(std::string message, Location location)
    : std::runtime_error(std::format("LexicalError [{}]: {}", location, message)),
      message(std::move(message)),
      location(location) {}

SyntaxError::SyntaxError(std::string message, Location location)
    : std::runtime_error(std::format("SyntaxError [{}]: {}", location, message)),
      message(std::move(message)),
      location(location) {}

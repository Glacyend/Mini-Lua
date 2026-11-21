#include "errors.h"

#include <format>
#include <utility>

LexicalError::LexicalError(std::string message, Location location)
    : std::runtime_error(
          (location.start == location.end)
              ? std::format("LexicalError at {} --- {}", location.start, message)
              : std::format(
                    "LexicalError at {} to {} --- {}", location.start, location.end, message
                )
      ),
      message(std::move(message)),
      location(location) {}

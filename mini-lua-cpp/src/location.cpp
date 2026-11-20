#include "location.h"

#include <cassert>

Position::Position(std::size_t line, std::size_t column)
    : line(line),
      column(column) {}

Location::Location(Position start, Position end)
    : start(start),
      end(end) {}

Location Location::combine(Location left, Location right) {
    assert(left.start <= right.end);
    return Location(left.start, right.end);
}

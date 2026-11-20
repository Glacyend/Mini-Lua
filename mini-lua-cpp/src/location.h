#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <compare>
#include <cstddef>

struct Position {
    std::size_t line = 0;
    std::size_t column = 0;

    Position(std::size_t line, std::size_t column);
    std::strong_ordering operator<=>(const Position& other) const = default;
    bool operator==(const Position& other) const = default;
};

struct Location {
    Position start;
    Position end;

    Location(Position start, Position end);
    bool operator==(const Location& other) const = default;

    static Location combine(Location left, Location right);
};

#endif

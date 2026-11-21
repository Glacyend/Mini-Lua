#include "token.h"

TokenReserved::TokenReserved(Location location, ReservedType type)
    : location(location),
      type(type) {}

TokenNumber::TokenNumber(Location location, double value)
    : location(location),
      value(value) {}

TokenString::TokenString(Location location, std::string value)
    : location(location),
      value(std::move(value)) {}

TokenSymbol::TokenSymbol(Location location, SymbolType type)
    : location(location),
      type(type) {}

TokenName::TokenName(Location location, std::string name)
    : location(location),
      name(std::move(name)) {}

TokenEOL::TokenEOL(Location location)
    : location(location) {}

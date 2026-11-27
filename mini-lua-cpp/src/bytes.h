#ifndef __BYTES_H__
#define __BYTES_H__

#include <array>
#include <cstdint>
#include <span>

std::array<std::uint8_t, 2> to_bytes(std::uint16_t value);
std::array<std::uint8_t, 4> to_bytes(std::uint32_t value);
std::array<std::uint8_t, 8> to_bytes(std::uint64_t value);
std::uint16_t from_bytes(std::span<const std::uint8_t, 2> bytes);
std::uint32_t from_bytes(std::span<const std::uint8_t, 4> bytes);
std::uint64_t from_bytes(std::span<const std::uint8_t, 8> bytes);

#endif

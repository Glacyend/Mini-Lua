#include "bytes.h"

std::array<std::uint8_t, 2> to_bytes(std::uint16_t value) {
    std::array<std::uint8_t, 2> result;
    result[0] = value & 0xFF;
    result[1] = (value >> 8) & 0xFF;
    return result;
}

std::array<std::uint8_t, 4> to_bytes(std::uint32_t value) {
    std::array<std::uint8_t, 4> result;
    result[0] = value & 0xFF;
    result[1] = (value >> 8) & 0xFF;
    result[2] = (value >> 16) & 0xFF;
    result[3] = (value >> 24) & 0xFF;
    return result;
}

std::array<std::uint8_t, 8> to_bytes(std::uint64_t value) {
    std::array<std::uint8_t, 8> result;
    result[0] = value & 0xFF;
    result[1] = (value >> 8) & 0xFF;
    result[2] = (value >> 16) & 0xFF;
    result[3] = (value >> 24) & 0xFF;
    result[4] = (value >> 32) & 0xFF;
    result[5] = (value >> 40) & 0xFF;
    result[6] = (value >> 48) & 0xFF;
    result[7] = (value >> 56) & 0xFF;
    return result;
}

std::uint16_t from_bytes(std::span<const std::uint8_t, 2> bytes) {
    std::uint16_t result = 0;
    result |= static_cast<std::uint16_t>(bytes[0]);
    result |= static_cast<std::uint16_t>(bytes[1]) << 8;
    return result;
}

std::uint32_t from_bytes(std::span<const std::uint8_t, 4> bytes) {
    std::uint32_t result = 0;
    result |= static_cast<std::uint32_t>(bytes[0]);
    result |= static_cast<std::uint32_t>(bytes[1]) << 8;
    result |= static_cast<std::uint32_t>(bytes[2]) << 16;
    result |= static_cast<std::uint32_t>(bytes[3]) << 24;
    return result;
}

std::uint64_t from_bytes(std::span<const std::uint8_t, 8> bytes) {
    std::uint64_t result = 0;
    result |= static_cast<std::uint64_t>(bytes[0]);
    result |= static_cast<std::uint64_t>(bytes[1]) << 8;
    result |= static_cast<std::uint64_t>(bytes[2]) << 16;
    result |= static_cast<std::uint64_t>(bytes[3]) << 24;
    result |= static_cast<std::uint64_t>(bytes[4]) << 32;
    result |= static_cast<std::uint64_t>(bytes[5]) << 40;
    result |= static_cast<std::uint64_t>(bytes[6]) << 48;
    result |= static_cast<std::uint64_t>(bytes[7]) << 56;
    return result;
}

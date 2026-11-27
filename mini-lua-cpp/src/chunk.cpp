#include "chunk.h"
#include "bytes.h"

#include <bit>
#include <utility>

std::uint8_t to_byte(Instruction instruction) {
    return static_cast<std::uint8_t>(instruction);
}

std::optional<Instruction> to_instruction(std::uint8_t byte) {
    std::uint8_t lowest = std::to_underlying(Instruction::Return);
    std::uint8_t highest = std::to_underlying(Instruction::LoadNumber);
    if (lowest <= byte && byte <= highest) {
        return Instruction(byte);
    } else {
        return std::nullopt;
    }
}

void Chunk::append_instruction(Instruction instruction) {
    append_byte(to_byte(instruction));
}

void Chunk::append_byte(std::uint8_t byte) {
    data.push_back(byte);
}

void Chunk::append_bytes(std::span<const std::uint8_t> bytes) {
    data.append_range(bytes);
}

void Chunk::append_number(double number) {
    auto bytes = to_bytes(std::bit_cast<std::uint64_t>(number));
    append_bytes(bytes);
}

std::optional<Instruction> Chunk::read_instruction(std::size_t index) const {
    return read_byte(index).and_then(to_instruction);
}

std::optional<std::uint8_t> Chunk::read_byte(std::size_t index) const {
    if (index < data.size()) {
        return data[index];
    } else {
        return std::nullopt;
    }
}

std::optional<std::vector<std::uint8_t>> Chunk::read_bytes(
    std::size_t index, std::size_t count
) const {
    if (index + count <= data.size()) {
        return std::vector(
            data.begin() + static_cast<std::ptrdiff_t>(index),
            data.begin() + static_cast<std::ptrdiff_t>(index + count)
        );
    } else {
        return std::nullopt;
    }
}

std::optional<double> Chunk::read_number(std::size_t index) const {
    if (index + 8 <= data.size()) {
        return std::bit_cast<double>(from_bytes(
            std::span<const std::uint8_t, 8>(data.begin() + static_cast<std::ptrdiff_t>(index), 8)
        ));
    } else {
        return std::nullopt;
    }
}

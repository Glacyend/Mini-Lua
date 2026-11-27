#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

enum class Instruction : std::uint8_t {
    Return,
    LoadNil,
    LoadTrue,
    LoadFalse,
    LoadNumber,
};

std::uint8_t to_byte(Instruction instruction);
std::optional<Instruction> to_instruction(std::uint8_t byte);

class Chunk {
private:
    std::vector<std::uint8_t> data;

public:
    Chunk() = default;
    void append_instruction(Instruction instruction);
    void append_byte(std::uint8_t byte);
    void append_bytes(std::span<const std::uint8_t> bytes);
    void append_number(double number);
    std::optional<Instruction> read_instruction(std::size_t index) const;
    std::optional<std::uint8_t> read_byte(std::size_t index) const;
    std::optional<std::vector<std::uint8_t>> read_bytes(std::size_t index, std::size_t count) const;
    std::optional<double> read_number(std::size_t index) const;
};

#endif

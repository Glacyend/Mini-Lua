#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "chunk.h"
#include "value.h"

#include <cstddef>

class ByteCode {
private:
    std::vector<Value> constants;
    Chunk chunk;

public:
    ByteCode() = default;
    const std::vector<Value>& get_constants() const;
    const Chunk& get_chunk() const;
    std::size_t append_constant(Value value);
    void append_instruction(Instruction instruction);
    void append_number(double number);
};

#endif

#include "compiler.h"

const std::vector<Value>& ByteCode::get_constants() const {
    return constants;
}

const Chunk& ByteCode::get_chunk() const {
    return chunk;
}

std::size_t ByteCode::append_constant(Value value) {
    std::size_t index = constants.size();
    constants.push_back(value);
    return index;
}

void ByteCode::append_instruction(Instruction instruction) {
    chunk.append_instruction(instruction);
}

void ByteCode::append_number(double number) {
    chunk.append_number(number);
}

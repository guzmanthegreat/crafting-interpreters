/* Chapter 14 Challenge 2: 24-bit constant operands. */
void writeConstant(Chunk* chunk, Value value, int line) {
  int index = addConstant(chunk, value);
  if (index < 256) {
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, (uint8_t)index, line);
  } else {
    writeChunk(chunk, OP_CONSTANT_LONG, line);
    writeChunk(chunk, (uint8_t)(index & 0xff), line);
    writeChunk(chunk, (uint8_t)((index >> 8) & 0xff), line);
    writeChunk(chunk, (uint8_t)((index >> 16) & 0xff), line);
  }
}

/* The VM decodes the same little-endian 24-bit operand: */
uint32_t index = READ_BYTE();
index |= (uint32_t)READ_BYTE() << 8;
index |= (uint32_t)READ_BYTE() << 16;
push(vm.chunk->constants.values[index]);

/* Chapter 15 Challenge 3: dynamic stack outline. */
typedef struct {
  Chunk* chunk;
  uint8_t* ip;
  Value* stack;
  int stackCount;
  int stackCapacity;
} VM;

void push(Value value) {
  if (vm.stackCapacity < vm.stackCount + 1) {
    int oldCapacity = vm.stackCapacity;
    vm.stackCapacity = GROW_CAPACITY(oldCapacity);
    vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity,
                           vm.stackCapacity);
  }
  vm.stack[vm.stackCount++] = value;
}

Value pop(void) {
  return vm.stack[--vm.stackCount];
}

/* The VM must free the dynamic stack during shutdown and use stackCount when
   printing/debugging the stack. In the later closure chapters, frame slots and
   open-upvalue locations must also be rebased after a reallocation. */

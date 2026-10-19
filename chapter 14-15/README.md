# Crafting Interpreters Chapters 14–15 Submission

This submission covers Chapters 14 and 15 of *Crafting Interpreters*.

## Contents

- `clox/` — C source code for the bytecode interpreter and virtual machine, including the integrated challenge changes.
- `answers/chapter14/` — written answers for all three Chapter 14 challenges.
- `answers/chapter15/` — written answers for all four Chapter 15 challenges.
- `tests/` — small Lox programs for testing the VM.

## Challenges covered

Chapter 14:

1. Run-length-style compression of line information.
2. `OP_CONSTANT_LONG` and 24-bit constant operands.
3. How open-source memory allocators work and how a simple allocator could be
   designed.

Chapter 15:

1. Bytecode sequences for expressions.
2. Removing `OP_NEGATE` or `OP_SUBTRACT` and evaluating the trade-off.
3. Dynamically growing the VM stack.
4. In-place stack operations and related optimizations.

## Compile and run

From the `clox` directory, compile the source files with a C compiler. For
example:

```sh
gcc -std=c99 -Wall -Wextra -O2 *.c -lm -o clox
./clox ../tests/arithmetic.lox
```

The included source integrates compressed line information, long constants,
dynamic stack growth, and in-place negation. The implementation notes explain
the changes and the written answers discuss their trade-offs.

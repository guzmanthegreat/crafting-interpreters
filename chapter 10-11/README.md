# Crafting Interpreters Chapters 10–11 Challenges

This submission contains:

- `jlox/`: the Java tree-walk interpreter source used for Chapters 10–11.
- `chapter_10-11_challenge_answers.md`: answers to all seven challenges,
  including the requested implementation code.
- `tests/`: Lox programs that exercise functions, closures, recursion, and
  resolver behavior.
- `chapter11_indexed_locals_reference/`: the complete indexed-environment
  implementation reference for Chapter 11 Challenge 4.

The Java source is the working jlox implementation from the project. Compile
it from the `jlox` directory with:

```sh
javac com/craftinginterpreters/lox/*.java
java com.craftinginterpreters.lox.Lox ../tests/chapter10_functions.lox
```

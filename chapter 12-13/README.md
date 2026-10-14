# Crafting Interpreters Chapters 12 and 13 Challenge Submission

Student: Josh Guzman  
Course: CS 4080.02, Concepts of Programming Languages  
Book: Robert Nystrom, *Crafting Interpreters*

## Included files

- `answers/Chapter12_Classes_Challenges.md`
- `answers/Chapter13_Inheritance_Challenges.md`
- `tests/Chapter12/static_methods.lox`
- `tests/Chapter12/getters.lox`
- `tests/Chapter13/beta_inner.lox`
- `tests/Chapter13/arrays_design.lox`
- `com/`, the provided Java jlox source used for the implementation discussion

The answer files explain the design and implementation changes required by each challenge. The test files use the exact Lox examples from the chapter challenges where the book provides them.

## Source limitation

The Java source in this ZIP is the provided interpreter snapshot. The challenge answers identify the files and methods that must change, including `Parser.java`, `Stmt.java`, `Interpreter.java`, `Resolver.java`, `LoxClass.java`, `LoxInstance.java`, `LoxFunction.java`, `Expr.java`, and `Environment.java`.

The source snapshot contains additional coursework features from earlier chapters. Only the sections related to Chapters 12 and 13 are discussed in the answer files.

## Sources

- https://craftinginterpreters.com/classes.html
- https://craftinginterpreters.com/inheritance.html

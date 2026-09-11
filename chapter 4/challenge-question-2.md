2. Aside from separating tokens—distinguishing 'print foo' from 'printfoo'—spaces aren’t used for much in most languages. However, in a couple of dark corners, a space does affect how code is parsed in CoffeeScript, Ruby, and the C preprocessor. Where and what effect does it have in each of those languages?

**Answer: CoffeeScript and Ruby permit some function or method calls without parentheses, so whitespace can help distinguish arguments from unary or binary operator expressions.
In the C preprocessor, whitespace between a macro name and '(' changes a function-like macro definition into an object-like macro definition.**

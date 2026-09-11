Most Lisp implementations that compile to C also contain an interpreter that lets them execute Lisp code on the fly as well. Why?

**Answer: Lisp can create and run new code while it is running. Its REPL must also execute expressions immediately. Compiling every expression to C would be slow. It would also require a C compiler at runtime. The interpreter runs this code immediately. The C compiler is used when better performance is needed.**

Just-in-time compilation tends to be the fastest way to implement a dynamically-typed language, but not all of them use it. What reasons are there to not JIT?

**Answer: JIT compilation has costs. It makes programs start more slowly. It also uses extra memory. The program needs time to warm up. Short programs may end before the JIT helps. JITs are also complex to build and maintain. An interpreter can be simpler and more predictable.**

3. Our scanner here, like most, discards comments and whitespace since those aren’t needed by the parser. Why might you want to write a scanner that does not discard those? What would it be useful for?

**Answer: A scanner should preserve comments and whitespace when building tools that must reproduce or analyze the original source text, including formatters, syntax highlighters, documentation generators, linters, refactoring tools, and source-to-source compilers.**

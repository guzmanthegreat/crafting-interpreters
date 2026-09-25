# Chapters 8 and 9 Challenge Answers

## Chapter 8

### 1. Expressions and statements in the REPL

The REPL now evaluates an expression entered without a semicolon and prints
its value. Statements still execute normally. For example, `1 + 2` prints
`3`, while `print 1 + 2;` executes as a Lox statement.

### 2. Uninitialized variables

A declaration without an initializer stores a private sentinel value instead
of treating the variable as initialized to `nil`. Reading the variable before
assigning it produces a runtime error. Explicitly assigning `nil` remains
valid.

### 3. Shadowing during initialization

In this program:

```lox
var a = 1;
{
  var a = a + 2;
  print a;
}
```

The inner `a` shadows the outer `a`, so the initializer tries to read the new
inner variable before it has been initialized. The sensible behavior is to
report an error instead of silently using the outer variable.

## Chapter 9

### 1. Conditional execution

With first-class functions and dynamic dispatch, a boolean can choose which
function to execute. Conceptually:

```text
condition.ifTrue(trueFunction, falseFunction)
```

The `true` object executes the first function and the `false` object executes
the second. Smalltalk uses this style with `ifTrue:` and `ifFalse:`.

### 2. Looping

Looping can be expressed with recursion. Tail-call optimization is needed so
that a recursive call in the final position reuses the current stack frame.
Without it, a long-running loop would eventually overflow the call stack.
Scheme is a language known for requiring proper tail calls.

### 3. `break`

Lox now supports `break;` inside `while` and `for` loops. The parser rejects
`break` outside a loop. The interpreter uses a lightweight internal exception
to leave nested blocks and conditionals and stop the nearest loop.

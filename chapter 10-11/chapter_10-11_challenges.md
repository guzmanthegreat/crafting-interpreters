# Chapters 10 and 11 Challenge Answers

These answers follow the Java tree-walk interpreter (jlox) described in
*Crafting Interpreters*.

## Chapter 10: Functions

### 1. Why does Smalltalk not need a runtime arity check?

Smalltalk uses keyword selectors. The pieces of a method name identify where
its arguments go, so the number of arguments is part of the method's name.
For example, a two-argument call can be written as:

```smalltalk
list insert: "element" at: 2
```

The complete selector is `insert:at:`, which has exactly two argument slots.
There is no separate comma-separated argument list that could contain too many
or too few values. The message lookup itself therefore fails if the selector
does not match a method with that arity.

### 2. Add anonymous functions (lambdas)

The implementation needs an expression node for a function body, while a
named function statement stores a name plus that expression.

Add this expression type in `GenerateAst.java`:

```java
"Function : List<Token> parameters, List<Stmt> body",
```

Change the function statement definition to:

```java
"Function : Token name, Expr.Function function",
```

Refactor the parser so the shared function-body parser handles both named and
anonymous functions:

```java
private Stmt.Function function(String kind) {
  Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
  return new Stmt.Function(name, functionBody(kind));
}

private Expr.Function functionBody(String kind) {
  consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
  List<Token> parameters = new ArrayList<>();

  if (!check(RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255) {
        error(peek(), "Can't have more than 255 parameters.");
      }
      parameters.add(consume(IDENTIFIER, "Expect parameter name."));
    } while (match(COMMA));
  }

  consume(RIGHT_PAREN, "Expect ')' after parameters.");
  consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
  return new Expr.Function(parameters, block());
}
```

In `primary()`, allow an anonymous function wherever a primary expression is
valid:

```java
if (match(FUN)) return functionBody("function");
```

To keep `fun name(...) { ... }` as a declaration while allowing `fun (...) {
... }` as an expression statement, use one-token lookahead:

```java
private boolean checkNext(TokenType type) {
  if (current + 1 >= tokens.size()) return false;
  return tokens.get(current + 1).type == type;
}
```

Then in `declaration()`:

```java
if (check(FUN) && checkNext(IDENTIFIER)) {
  consume(FUN, null);
  return function("function");
}
```

Finally, make `LoxFunction` store the shared function expression and an
optional name. Named functions print as `<fn name>`; lambdas print as `<fn>`.
The interpreter creates them like this:

```java
@Override
public Void visitFunctionStmt(Stmt.Function stmt) {
  String name = stmt.name.lexeme;
  environment.define(name,
      new LoxFunction(name, stmt.function, environment));
  return null;
}

@Override
public Object visitFunctionExpr(Expr.Function expr) {
  return new LoxFunction(null, expr, environment);
}
```

This makes the following work:

```lox
fun thrice(fn) {
  for (var i = 1; i <= 3; i = i + 1) fn(i);
}

thrice(fun(a) { print a; });
```

Output:

```text
1
2
3
```

### 3. Are parameters and local variables in the same scope?

No, this program is not valid in Lox:

```lox
fun scope(a) {
  var a = "local";
}
```

The parameters and the variables declared directly in the function body share
one scope, so declaring `a` again is a duplicate declaration.

Java and C also reject this kind of collision. Dart allows it because it treats
the parameters as being in an outer scope around the function body. The Lox
choice is preferable here because silently shadowing a parameter makes a
function harder to understand.

## Chapter 11: Resolving and Binding

### 1. Why is a function name defined eagerly?

This is safe because a function cannot be called from outside until its
declaration has finished executing. Recursive calls made inside the function
body are delayed until after the declaration has bound the function name.

For example, the first call to `foo()` must come from code outside the
declaration:

```lox
fun foo() {
  if (condition) foo();
}
```

By the time that outside call occurs, `foo` is already defined. The function
body may therefore refer to its own name safely.

### 2. What happens when a local variable refers to itself in its initializer?

For:

```lox
var a = "outer";
{
  var a = a;
}
```

Lox should report a compile-time resolution error. The inner `a` is declared
before its initializer is resolved, so the initializer sees the inner variable,
but that variable has not been defined yet. Reading it is invalid.

This is safer than C, where the read can produce an indeterminate value. Java
also rejects the local-variable case through definite-assignment analysis.
Globals may have different rules in other languages, but using one consistent
rule for local declarations makes Lox easier to reason about.

### 3. Report local variables that are never used

Replace each scope entry's Boolean with a three-state record:

```java
private static class Variable {
  final Token name;
  VariableState state;

  Variable(Token name, VariableState state) {
    this.name = name;
    this.state = state;
  }
}

private enum VariableState {
  DECLARED, DEFINED, READ
}
```

Use `Stack<Map<String, Variable>>` for the scope stack. Declarations start in
the `DECLARED` state, then `define()` changes them to `DEFINED`. A variable
expression changes the state to `READ`; an assignment alone does not count as
using a variable.

```java
private void endScope() {
  Map<String, Variable> scope = scopes.pop();
  for (Variable variable : scope.values()) {
    if (variable.state == VariableState.DEFINED) {
      Lox.error(variable.name, "Local variable is not used.");
    }
  }
}
```

The resolver should also keep the existing self-initializer check, now using
`VariableState.DECLARED`:

```java
if (!scopes.isEmpty()
    && scopes.peek().containsKey(expr.name.lexeme)
    && scopes.peek().get(expr.name.lexeme).state == VariableState.DECLARED) {
  Lox.error(expr.name,
      "Can't read local variable in its own initializer.");
}
```

### 4. Resolve locals by array index instead of name

Each scope can assign a slot number as variables are declared. The resolver
stores both the lexical distance and the slot number for every local access.
The interpreter then uses `List<Object>` environments and performs indexed
access instead of a map lookup.

The essential changes are:

```java
private class Variable {
  boolean isDefined = false;
  final int slot;

  Variable(int slot) {
    this.slot = slot;
  }
}
```

When declaring a variable, assign the next available slot:

```java
int slot = scope.size();
scope.put(name.lexeme, new Variable(slot));
```

When resolving an expression, store both values:

```java
interpreter.resolve(expr, distance, variable.slot);
```

The environment stores values in an array-like list:

```java
private final List<Object> values = new ArrayList<>();

void define(Object value) {
  values.add(value);
}

Object getAt(int distance, int slot) {
  Environment environment = this;
  for (int i = 0; i < distance; i++) {
    environment = environment.enclosing;
  }
  return environment.values.get(slot);
}

void assignAt(int distance, int slot, Object value) {
  Environment environment = this;
  for (int i = 0; i < distance; i++) {
    environment = environment.enclosing;
  }
  environment.values.set(slot, value);
}
```

The interpreter's local-resolution map therefore stores a slot as well as a
distance, and variable reads and assignments call `getAt(distance, slot)` and
`assignAt(distance, slot, value)`. Global variables can remain name-based.

This completes the optimization without changing Lox's visible behavior: the
resolver performs the name search once, before execution, and the interpreter
uses the resulting numeric location during execution.

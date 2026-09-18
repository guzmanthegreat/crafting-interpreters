# Challenge 1: Adding the Comma Operator to Lox

## Grammar

I added the comma operator as the lowest-precedence expression in Lox. The grammar I used is:

```text
expression -> comma ;
comma      -> assignment ( "," assignment )* ;
```

The comma operator is left-associative. It evaluates the expression on the left, discards that result, and returns the result of the expression on the right.

For example:

```lox
print (1 + 2, 3 * 4);
```

The interpreter evaluates `1 + 2` first and discards its result. It then evaluates `3 * 4` and prints `12`.

## Parser.java

I changed `expression()` so it starts with the new comma rule:

```java
private Expr expression() {
  return comma();
}
```

I added this method directly below `expression()`:

```java
private Expr comma() {
  Expr expr = assignment();

  while (match(COMMA)) {
    Token operator = previous();
    Expr right = assignment();
    expr = new Expr.Binary(expr, operator, right);
  }

  return expr;
}
```

The `while` loop allows more than one comma, such as `1, 2, 3`. Reusing the previous expression as the left operand makes the operator left-associative.

I also changed the argument parsing line in `finishCall()` from:

```java
arguments.add(expression());
```

to:

```java
arguments.add(assignment());
```

This keeps commas between function arguments separate from the comma operator. For example, `add(10, 20)` is still parsed as a call with two arguments.

## Interpreter.java

I added the following case inside the `switch` in `visitBinaryExpr()`:

```java
case COMMA:
  return right;
```

The method evaluates both operands before entering the switch. Returning `right` discards the left result and gives the comma expression the value of its right operand.

## Testing

I compiled the project with:

```bash
javac com/craftinginterpreters/lox/*.java
```

I then ran the interpreter with:

```bash
java com.craftinginterpreters.lox.Lox
```

I tested the following expressions:

```lox
print (1, 2);
// Output: 2

print (1 + 2, 3 * 4, 20 - 5);
// Output: 15
```

These results show that every expression is evaluated and that the value of the final expression is returned.

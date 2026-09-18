# Challenge 3: Error Productions for Missing Left Operands

## Goal

I added error productions for binary operators that appear at the beginning of an expression without a left-hand operand.

For example, this expression is invalid:

```lox
print + 5;
```

The `+` operator has a right operand (`5`), but it does not have a left operand. Instead of producing the general message `Expect expression.`, the parser now reports:

```text
Missing left-hand operand.
```

After reporting the error, the parser consumes and discards the right operand at the correct precedence level. This helps prevent additional misleading errors.

## Parser.java

I added the error productions inside `unary()`, after valid unary operators are handled and before the parser calls `primary()`.

```java
private Expr unary() {
  if (match(BANG, MINUS)) {
    Token operator = previous();
    Expr right = unary();
    return new Expr.Unary(operator, right);
  }

  if (match(COMMA)) {
    error(previous(), "Missing left-hand operand.");
    assignment();
    return new Expr.Literal(null);
  }

  if (match(OR)) {
    error(previous(), "Missing left-hand operand.");
    and();
    return new Expr.Literal(null);
  }

  if (match(AND)) {
    error(previous(), "Missing left-hand operand.");
    equality();
    return new Expr.Literal(null);
  }

  if (match(BANG_EQUAL, EQUAL_EQUAL)) {
    error(previous(), "Missing left-hand operand.");
    comparison();
    return new Expr.Literal(null);
  }

  if (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    error(previous(), "Missing left-hand operand.");
    term();
    return new Expr.Literal(null);
  }

  if (match(PLUS)) {
    error(previous(), "Missing left-hand operand.");
    factor();
    return new Expr.Literal(null);
  }

  if (match(SLASH, STAR)) {
    error(previous(), "Missing left-hand operand.");
    unary();
    return new Expr.Literal(null);
  }

  return call();
}
```

Each error production calls the parsing method for the operator's right operand:

| Operator | Method used for the right operand |
|---|---|
| `,` | `assignment()` |
| `or` | `and()` |
| `and` | `equality()` |
| `==`, `!=` | `comparison()` |
| `>`, `>=`, `<`, `<=` | `term()` |
| `+` | `factor()` |
| `*`, `/` | `unary()` |

The parser returns `new Expr.Literal(null)` as a placeholder. The interpreter does not execute the expression because reporting the syntax error sets Lox's error flag.

## Unary Minus Exception

I did not treat `-` at the beginning of an expression as an error because Lox supports unary negation:

```lox
print -5;
```

This is a valid expression. The parser cannot interpret the starting `-` as a binary operator missing its left operand because the same symbol is valid as a unary operator.

## Test Cases

Each of these should report `Missing left-hand operand.` at the operator:

```lox
print + 5;
print * 5;
print / 5;
print == 5;
print != 5;
print > 5;
print >= 5;
print < 5;
print <= 5;
```

These additional tests cover operators added in later chapters:

```lox
print , 5;
print and true;
print or false;
```

This expression should remain valid:

```lox
print -5;
// Expected output: -5
```

# Challenge 2: Adding the Conditional Operator to Lox

## Grammar

I added the C-style conditional operator `?:` to Lox. The grammar I used is:

```text
expression  -> comma ;
comma       -> assignment ( "," assignment )* ;
assignment  -> IDENTIFIER "=" assignment | conditional ;
conditional -> or ( "?" expression ":" conditional )? ;
```

The expression between `?` and `:` is parsed using `expression()`. This allows an expression of any precedence in the middle, including a comma expression. The expression after `:` is parsed using `conditional()`, which makes the conditional operator right-associative.

For example:

```lox
a ? b : c ? d : e
```

is grouped as:

```lox
a ? b : (c ? d : e)
```

## TokenType.java and Scanner.java

I added token types for the two new symbols:

```java
QUESTION, COLON
```

I added these cases to `Scanner.scanToken()`:

```java
case '?': addToken(QUESTION); break;
case ':': addToken(COLON); break;
```

## Expr.java

I added a visitor method:

```java
R visitConditionalExpr(Conditional expr);
```

I also added a `Conditional` expression class containing the condition, true branch, and false branch:

```java
static class Conditional extends Expr {
  Conditional(Expr condition, Expr thenBranch, Expr elseBranch) {
    this.condition = condition;
    this.thenBranch = thenBranch;
    this.elseBranch = elseBranch;
  }

  @Override
  <R> R accept(Visitor<R> visitor) {
    return visitor.visitConditionalExpr(this);
  }

  final Expr condition;
  final Expr thenBranch;
  final Expr elseBranch;
}
```

## Parser.java

I changed `assignment()` so that it calls `conditional()` instead of `or()`:

```java
Expr expr = conditional();
```

I added this parsing method:

```java
private Expr conditional() {
  Expr expr = or();

  if (match(QUESTION)) {
    Expr thenBranch = expression();
    consume(COLON,
        "Expect ':' after then branch of conditional expression.");
    Expr elseBranch = conditional();
    expr = new Expr.Conditional(expr, thenBranch, elseBranch);
  }

  return expr;
}
```

## Interpreter.java

I added the following visitor method:

```java
@Override
public Object visitConditionalExpr(Expr.Conditional expr) {
  if (isTruthy(evaluate(expr.condition))) {
    return evaluate(expr.thenBranch);
  }

  return evaluate(expr.elseBranch);
}
```

This evaluates the condition first and evaluates only the selected branch. The unselected branch is not evaluated.

## Resolver.java

I added a visitor that resolves all three subexpressions:

```java
@Override
public Void visitConditionalExpr(Expr.Conditional expr) {
  resolve(expr.condition);
  resolve(expr.thenBranch);
  resolve(expr.elseBranch);
  return null;
}
```

## AstPrinter.java

I added support for displaying the conditional syntax tree:

```java
@Override
public String visitConditionalExpr(Expr.Conditional expr) {
  return parenthesize("?:", expr.condition,
                      expr.thenBranch, expr.elseBranch);
}
```

## Test Cases

I used the following test cases to check both branches, nesting, precedence, compatibility with the comma operator, and short-circuit behavior:

```lox
print true ? 10 : 20;
// Expected: 10

print false ? 10 : 20;
// Expected: 20

print false ? 1 : true ? 2 : 3;
// Expected: 2

print true ? 1 + 2 : 3 * 4;
// Expected: 3

print (1, true ? 7 : 8);
// Expected: 7

print true ? "safe" : 1 / "bad";
// Expected: safe
```

The final test verifies short-circuit behavior. The invalid division is in the unselected branch, so it should not be evaluated.

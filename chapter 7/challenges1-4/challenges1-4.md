# Evaluating Expressions: Challenge Questions

## Challenge 1: Comparisons Between Different Types

I would not extend Lox's ordered comparison operators (`<`, `<=`, `>`, and `>=`) to mixed types. I would continue allowing these operators only when both operands are numbers.

I made this choice because there is no clear or natural way to decide whether a number such as `3` should be less than a string such as `"pancake"`. Lox would need an arbitrary type-ordering rule, and programmers would have to memorize it. Automatic conversions could also hide mistakes instead of reporting them.

I would still allow equality operators (`==` and `!=`) to compare values of any type. Values with different types would simply not be equal. This is reasonable because equality asks whether two values are the same, while ordered comparison requires a meaningful definition of which value comes first.

This approach is similar to Python 3, which reports an error for ordered comparisons between incompatible types such as a number and a string. Java also prevents many incompatible comparisons through static type checking. JavaScript performs more automatic type conversion, but that flexibility can produce results that are difficult to predict. I prefer the safer and simpler rule for Lox.

## Challenge 2: String Concatenation With `+`

I extended `visitBinaryExpr()` so that if either operand of `+` is a string, the other operand is converted to a string and the two strings are concatenated.

The updated `PLUS` case is:

```java
case PLUS:
  if (left instanceof Double && right instanceof Double) {
    return (double)left + (double)right;
  }

  if (left instanceof String || right instanceof String) {
    return stringify(left) + stringify(right);
  }

  throw new RuntimeError(expr.operator,
      "Operands must be two numbers or include a string.");
```

Numbers are still added normally when both operands are numbers. When either operand is a string, `stringify()` converts both values to their Lox text representation before concatenation.

Example test cases:

```lox
print "scone" + 4;
// Expected: scone4

print 4 + "scones";
// Expected: 4scones

print "ready: " + true;
// Expected: ready: true

print "value: " + nil;
// Expected: value: nil

print 2 + 3;
// Expected: 5
```

## Challenge 3: Division by Zero

Before this change, Lox used Java `double` arithmetic. Dividing a positive floating-point number by zero produced `Infinity`, dividing a negative number by zero produced `-Infinity`, and `0 / 0` produced `NaN`. Java follows the IEEE 754 floating-point standard for these results.

I chose to make division by zero a Lox runtime error. Although infinity can be useful in some mathematical programs, division by zero usually indicates a mistake. A clear runtime error tells the programmer exactly what went wrong instead of allowing an invalid value to affect later calculations.

Different languages make different choices. Python raises `ZeroDivisionError`. Java throws `ArithmeticException` for integer division by zero, but its floating-point division produces infinity or `NaN`. JavaScript also uses floating-point numbers and normally produces `Infinity`, `-Infinity`, or `NaN`. I chose behavior closer to Python because it is easier to detect and correct mistakes.

I updated the `SLASH` case in `visitBinaryExpr()`:

```java
case SLASH:
  checkNumberOperands(expr.operator, left, right);
  if ((double)right == 0.0) {
    throw new RuntimeError(expr.operator,
        "Cannot divide by zero.");
  }
  return (double)left / (double)right;
```

The comparison with `0.0` also detects negative zero because Java considers `-0.0 == 0.0` to be true.

Example test cases:

```lox
print 10 / 2;
// Expected: 5

print 10 / 0;
// Expected runtime error: Cannot divide by zero.

print 0 / 0;
// Expected runtime error: Cannot divide by zero.

print 10 / -0;
// Expected runtime error: Cannot divide by zero.
```

## Files Changed

Only `Interpreter.java` needed to be changed for the implementation portions of these challenges. Challenge 1 is a language-design response and does not require a code change.

# Chapter 12, Classes, Challenge Answers

## 1. Static methods

The requested Lox syntax is:

```lox
class Math {
  class square(n) {
    return n * n;
  }
}

print Math.square(3); // Prints "9".
```

I would implement this with metaclasses, which is also the approach described in the interpreter's challenge answer notes. A class object becomes an instance of another `LoxClass`, its metaclass. The metaclass stores the class methods.

The implementation changes are:

1. Add a second method list, `classMethods`, to `Stmt.Class`.
2. In `Parser.classDeclaration()`, consume the optional `class` keyword before a method and place that method in `classMethods`.
3. Resolve class methods in `Resolver.visitClassStmt()`. Their `this` refers to the class object.
4. Make `LoxClass` extend `LoxInstance` and give each class object a metaclass.
5. In `Interpreter.visitClassStmt()`, create a metaclass for the class methods, then create the ordinary class object using that metaclass.
6. Reuse `LoxInstance.get()`. Since a class object is now a `LoxInstance`, `Math.square` follows the normal property lookup path and finds `square` on the metaclass.

The important design point is that static methods are still methods. The only difference is the receiver. An ordinary method receives an instance as `this`. A static method receives the class object as `this`.

## 2. Getters

The requested Lox syntax is:

```lox
class Circle {
  init(radius) {
    this.radius = radius;
  }

  area {
    return 3.141592653 * this.radius * this.radius;
  }
}

var circle = Circle(4);
print circle.area; // Prints roughly "50.2655".
```

The parser must distinguish a getter from an ordinary method with zero parameters. The clean representation is:

- `params == null` means getter
- an empty parameter list means an ordinary method with zero parameters

The implementation changes are:

1. In `Parser.function("method")`, allow the parameter list to be omitted only for methods.
2. In `Resolver.resolveFunction()`, skip parameter resolution when the parameter list is `null`.
3. In `LoxFunction.call()`, skip parameter binding when the parameter list is `null`.
4. Add `isGetter()` to `LoxFunction`, returning `declaration.params == null`.
5. In `Interpreter.visitGetExpr()`, look up the property. If the result is a getter, immediately call it and return its result.

The important behavior is that `circle.area` runs code immediately. It does not return a function that requires parentheses.

## 3. Encapsulation trade-offs

Lox currently allows code outside a class to read and write fields directly. This makes the class easier to use because callers can inspect and change state without extra methods. It also makes experimentation quick.

The cost is stronger coupling. If callers depend directly on the internal field layout, the class author cannot change that implementation without potentially breaking them. Direct access can also allow callers to create invalid states.

Encapsulation makes the class responsible for protecting its own state. Methods, getters, and setters can validate values and preserve invariants. The cost is more code and a more restricted interface for users.

My answer is that a small teaching language can reasonably allow public fields because the feature is simple and flexible. A larger application benefits more from encapsulation because stable public methods allow the class implementation to change without breaking callers.

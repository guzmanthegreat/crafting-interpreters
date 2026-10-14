# Chapter 13, Inheritance, Challenge Answers

## 1. Traits

I would add traits. A trait is a reusable group of methods, but it is not a class and cannot be constructed. A class can include multiple traits with a `with` clause.

Example syntax:

```lox
trait Printable {
  printValue() {
    print this.value;
  }
}

class Report with Printable {
}
```

The implementation would add `TRAIT` and `WITH` tokens, a trait statement in the AST, and a list of traits on `Stmt.Class`. The parser would read trait declarations and the optional `with` clause. The resolver would resolve each trait expression and create the normal `this` scope for trait methods.

At runtime, a `LoxTrait` object would store its methods. When a class is created, the interpreter would copy methods from each included trait into the class method map. A collision between two traits would be an error instead of silently choosing one method. This keeps method reuse explicit and avoids the ambiguity of multiple inheritance.

## 2. BETA-style `inner`

The challenge reverses Lox's normal method lookup. The highest method in the inheritance chain wins. A superclass method can call `inner()` to invoke the next method down.

The supplied challenge example should print:

```text
Fry until golden brown.
Pipe full of custard and coat with chocolate.
Place in a nice box.
```

The implementation needs to remember two methods during lookup:

```java
LoxFunction findMethod(LoxInstance instance, String name) {
  LoxFunction method = null;
  LoxFunction inner = null;
  LoxClass klass = this;

  while (klass != null) {
    if (klass.methods.containsKey(name)) {
      inner = method;
      method = klass.methods.get(name);
    }
    klass = klass.superclass;
  }

  if (method != null) return method.bind(instance, inner);
  return null;
}
```

The method must continue walking upward instead of stopping at the first match. The top method is returned, and the next method down is stored as `inner` in the closure environment. The resolver also defines `inner` in the method scope. `LoxClass.call()` must use the new lookup function for `init()` as well.

## 3. Add arrays

I would add fixed-size arrays as a native Lox feature. The user creates one with `Array(size)`. Each element begins as `nil`.

Example behavior:

```lox
var array = Array(3);
print array.length; // 3
array.set(1, "new");
print array.get(1); // new
```

The interpreter would add a native `Array` callable and a `LoxArray` runtime class containing an `Object[]`. `get`, `set`, and `length` would be exposed as native properties. Index bounds should raise a runtime error, and arrays should reject arbitrary new fields.

Arrays are useful because field-based objects do not provide constant-time indexed storage. A contiguous array also provides the foundation for implementing other data structures such as lists, stacks, queues, and hash tables.

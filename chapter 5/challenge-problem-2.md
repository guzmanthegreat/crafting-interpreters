## Question

Create the opposite of the Visitor pattern for a functional language. It should group all operations for one type together and make new types easy to add.

## Answer

The Visitor pattern makes it easy to add a new operation, such as printing or evaluating an expression.

The opposite pattern would make it easy to add a new expression type. In Haskell, I can do this with a type class:

class Expression a where
evaluate :: a -> Double
display :: a -> String

data Literal = Literal Double

instance Expression Literal where
evaluate (Literal value) = value
display (Literal value) = show value

data Negative = Negative Double

instance Expression Negative where
evaluate (Negative value) = -value
display (Negative value) = "(- " ++ show value ++ ")"

Each new type keeps its functions together. I can add another type without changing the old types. The downside is that adding a new function means I have to update every type.

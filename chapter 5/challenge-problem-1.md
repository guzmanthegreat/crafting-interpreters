## Question

Earlier, I said that the |, \*, and + forms we added to our grammar metasyntax were just syntactic sugar. Take this grammar:

expr → expr ( "(" ( expr ( "," expr )_ )? ")" | "." IDENTIFIER )+
| IDENTIFIER
| NUMBER
Produce a grammar that matches the same language but does not use any of that notational sugar.
Answer:
expr → expr ( "(" ( expr ( "," expr )_ )? ")" | "." IDENTIFIER )+
| IDENTIFIER
| NUMBER

## Answer:

expr → expr call ;
expr → expr property ;
expr → IDENTIFIER ;
expr → NUMBER ;

call → "(" ")" ;
call → "(" arguments ")" ;

arguments → expr ;
arguments → expr "," arguments ;

property → "." IDENTIFIER ;

Each possible choice now has its own rule. Recursion replaces \* and +. For example, arguments can keep referring to itself to create more arguments:

function(1, 2, 3)

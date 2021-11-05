Random Packing Templates

* .mrt files: Macveth Random Templates

Their syntax is quite simple:

line ::= <expr>
<expr> ::= <reg> = <inst> | <inst>
<inst> ::= signature(<args>)
<args> ::= <reg> | <mem> | *args

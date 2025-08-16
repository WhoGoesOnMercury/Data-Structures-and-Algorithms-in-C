## Hash Table

Think of a hash table as a finite one-dimensional array. Like all arrays, an 
index is simply an integer, which means that it can be mathematically calculated. 
To calculate an index in this array, we use a function `f(x)` that accepts an input 
`k` which then outputs a value `h`. 

                                        f(k) = h

`k` is what we will refer to as a `key`

`h` is the `hash`

`f(x)` is the `hashing function`

Optimally, each hash should only be calculable through a single unique key but
this is never the case (only in theory!). Since a hash is calculated through a 
mathematically, it is most likely that there is a set of input keys that output
the same hash. 

I do not delve onto hashing functions here.

### Implementation

This is a hashtable implementation in C based on James Routley's tutorial. A 
more in-depth description of the codebase can be located in the link below. When
you run the Makefile, you will be greeted with "defined but not used" warnings. 
They are minor and are simply there because—redundantly—those functions were not 
called or used in any form in the program. 
        
Here is a link to the Github repository: https://github.com/jamesroutley/write-a-hash-table

Here is a link to an e-portfolio-like website of his: https://routley.io/


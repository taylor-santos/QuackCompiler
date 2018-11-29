# Quack C Compiler(qcc)
##### A Fully Functional Quack Compiler
##### By Alonzo Altamirano & Taylor Santos
---
### Our Interpretations of Quack Language Ambiguities:
1. **Constructors don't have explicit return statements.**  We believe that treating a constructor as any old method raises additional questions.  Can a constructor return a subtype of the class it is defined for?  When collecting fields, wouldn't all fields following the first encounterable return statement be moot?  We answer these questions by simply forbidding explicit return statements and by appending an implicit `return this` at the end of a constructor.  As a result, good_adv_constructor_init.qk yields an error.
2. **Negation syntactic sugar is not only for Ints.** Our version of negation does not impose the restriction that x be an integer in `-x`.  We do not treat negation as `0.MINUS(x)` but rather as `x.NEGATE()`.  Therefore, any class may declare a NEGATE() method and utilize the expressiveness of this syntactic sugar.
3. **Explicitly typed variables may have dynamic type that is a subtype of said explicit type.**  In addition, variables may be declared an explicit type multiple times, as long as the type is the same each time.
4. **Typecase type alt variables types are added to method scope.** Type alternatives in a type case must have fresh identifiers.  The type alternatives are added to the method scope with their explicitly defined type, but are not initialized in the typecase.
5. **Other than field access, "this" cannot be used in constructor r_expr's.**  One may not assign 'this', use 'this' in assignment, call methods on 'this', or use 'this' when calling methods or constructors.
6. **Identifiers cannot be overloaded... within the same scope.**  Identifiers in one class, such as fields, variables, and methods,  may not use the identifiers that serve as any existing class name.  Furthermore, within any class, no two fields, variables, and/or methods can share the same identifier.  For example: No class may have a field `this.PRINT`.  A class descended from `Obj` alone may have a field `this.PLUS`.  No class can have a field `this.Obj`.
7. **Program body cannot have explicit return statements.** Just that.
8. **Builtin Classes have empty constructors with no arguements.** Calling the constructor of builtin types, such as Obj(), String(), Int(), Boolean(), Nothing() return objects of those types.  The exact behavior is given below.

|Type|Value|
|:-------:|:-----:|
|   Obj   | empty |
|   Int   |   0   |
|  String |   ""  |
| Boolean | false |
| Nothing |  none |
---
## Credit Where Credit is Due
Cloned from https://github.com/UO-cis561/reflex-bison-ast. Modified to parse Quack (.qk) source code. 

---
## Building
Run the following commands from the project's root directory:
```
cmake .
make
```
---
## "Man Page"
###### Some Rules:
* *DO NOT MIX FLAGS*
* *FLAGS COME BEFORE QUACK FILENAME*
---

  ./qcc   [-v] | [-e] | [-c filename] | [-o filename] filename
  Running qcc with a single arguement, a .qk file, will produce a C file of the same name in the current directory.  
* -v: verbose, print all AST information. Symbol Tables, JSON, Class Heirarchy, ... the whole nine yards.
* -e: executable, compile to an executable named `a.out`
* -c: named c file, the filename following this command will name the c file outputted by qcc.
* -o: named executable, the filename following this command will name the executable.

## Examples
* `./qcc demo/sort.qk` "Produces the file sort.c in the current directory."
* `./qcc -v demo/sort.qk` "Prints the AST information to stdout."
* `./qcc -e demo/sort.qk` "Produces the executable a.out and the file sort.c in the current directory."
* `./qcc -c my_file.c demo/sort.qk` "Produces the file my_file.c in the current directory."
* `./qcc -o my_exec demo/sort.qk` "Produces the executable my_exec and the file sort.c in the current directory."

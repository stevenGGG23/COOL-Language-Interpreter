# COOL Language Interpreter

A tree-walking interpreter for the **COOL** (Classroom Object-Oriented Language) programming language, built in C++. This project evaluates a COOL program by traversing its abstract syntax tree (AST) and computing the result of each expression at runtime.

---

## What is COOL?

COOL is a statically typed, object-oriented language developed at UC Berkeley for teaching compiler and interpreter design. It supports classes, inheritance, pattern matching, arithmetic, and I/O. This interpreter takes a valid COOL source file, parses it into an AST, type-checks it, and then evaluates it by walking the tree.

---

## Project Structure

```
.
├── Absyn.h                  # AST node class definitions
├── AbsynExtension.h         # Extensions to AST nodes (interp hooks)
├── Interpreter.h            # ValueEntry class and global symbol declarations
├── Interpreter.cpp          # Routes each interp() call to student or teacher version
├── Interpreter_student.cpp  # Core interpreter logic (student implementation)
├── Semant.h                 # Environment, ClassTable, InheritanceNode definitions
├── SymbolTable.h/.cpp       # Generic scoped symbol table
├── StringTab.h              # String/Id/Int table entries
├── lib.h                    # Built-in class method helpers (IO, String, Object)
├── main.cpp                 # Entry point: parse -> semant -> interp
├── dump.cpp                 # AST pretty-printer
├── lex.yy.cc                # Flex-generated lexer
├── COOL.tab.cc / .hh        # Bison-generated parser
├── libinterpreter.a         # Precompiled teacher solution library
├── makefile                 # Build configuration
├── good.cl                  # Valid COOL test program
└── bad.cl                   # COOL program with intentional semantic errors
```

---

## How It Works

The pipeline has three stages:

1. **Parsing** — The lexer and parser (Flex + Bison) read the `.cl` source file and construct an AST made up of nodes defined in `Absyn.h`.

2. **Type Checking / Semantic Analysis** — `semant()` walks the AST, builds an inheritance graph, populates symbol tables, and verifies type correctness. Errors are reported with file, line, and column information.

3. **Interpretation** — `interp()` is called on each AST node recursively. Each expression evaluates to a `ValueEntry`, which holds a typed value: `Int`, `Bool`, `String`, `Void`, or an object (represented as an `Environment`).

### Expression Types Implemented

| Expression     | Description                                              |
|----------------|----------------------------------------------------------|
| `IntExp`       | Integer literal                                          |
| `BoolExp`      | Boolean literal                                          |
| `StringExp`    | String literal                                           |
| `OpExp`        | Arithmetic and comparison: `+`, `-`, `*`, `/`, `<`, `<=`, `=` |
| `NotExp`       | Boolean negation                                         |
| `ObjectExp`    | Variable lookup, including `self`                        |
| `AssignExp`    | Variable assignment                                      |
| `IfExp`        | Conditional branching                                    |
| `WhileExp`     | Loop with predicate                                      |
| `BlockExp`     | Sequence of expressions; returns value of last           |
| `LetExp`       | Local variable binding with optional initializer         |
| `NewExp`       | Object instantiation                                     |
| `IsvoidExp`    | Void check                                               |
| `CallExp`      | Dynamic method dispatch (teacher implementation)         |
| `StaticCallExp`| Static method dispatch (teacher implementation)          |
| `CaseExp`      | Pattern matching on runtime type (teacher implementation)|

---

## Dependencies

- **g++** with C++11 or later
- **Flex** and **Bison** (pre-generated files included)
- Linux or macOS (the included `libinterpreter.a` was compiled for Linux/x86-64)

---

## Building

```bash
make
```

This compiles all source files and links against `libinterpreter.a` to produce the `main` executable.

To clean build artifacts:

```bash
make clean
```

---

## Running

```bash
./main <filename.cl>
```

### Examples

Run the provided valid test program:

```bash
./main good.cl
```

Run the program with intentional semantic errors (should report errors and halt):

```bash
./main bad.cl
```

---

## Testing good.cl

`good.cl` is an interactive menu-driven program. After launching, it prompts you to enter a number and then choose an operation:

```
Please enter a number...  10

        To add a number to 10 ...enter a:
        To negate 10 ...enter b:
        To find the difference between 10 and another number...enter c:
        To find the factorial of 10 ...enter d:
        To square 10 ...enter e:
        To cube 10 ...enter f:
        To find out if 10 is a multiple of 3...enter g:
        To divide 10 by 8...enter h:
        To get a new number...enter j:
        To quit...enter q:
```

Each option exercises a different interpreter feature — arithmetic, conditionals, loops, recursion, and dynamic dispatch.

---

## Value Representation

All runtime values are instances of `ValueEntry`:

```cpp
ValueEntry()              // Void
ValueEntry(int v)         // Integer
ValueEntry(bool v)        // Boolean
ValueEntry(string s)      // String
ValueEntry(Symbol t, EnvironmentP e)  // Object of type t
```

Objects are represented as `Environment` pointers, which hold the object's attribute bindings in a scoped symbol table.

---

## Environment and Scoping

The interpreter uses a `SymbolTable<Symbol, ValueEntry>` for variable bindings. Scopes are pushed and popped explicitly:

- `var_enterScope()` / `var_exitScope()` — used in `LetExp`
- `var_value_lookup(name)` — searches all scopes
- `var_value_change(name, value)` — updates an existing binding
- `var_add(name, type)` — declares a new variable in the current scope

---

## Course Context

This project was completed as part of **CSCI 4160** (Compiler Design). It is the final stage of a multi-project compiler pipeline covering lexical analysis, parsing, semantic analysis, and interpretation of the COOL language.

---

## License

Source files derived from UC Berkeley COOL infrastructure are subject to the UC Berkeley copyright. See `copyright.h` for details. Student-authored code in `Interpreter_student.cpp` is original work.

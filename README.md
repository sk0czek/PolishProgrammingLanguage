# Polisz

Why? I was 17, it was summer, and apparently writing an interpreter in C was my idea of a good time.

A small tree-walk interpreter with Polish keywords, written in C.

## Example

```text
podaj(x);
wynik = (x + 5) * 2;

jezeli(wynik) {
    wyswietl(wynik);
}
```

Enter `7` to print `24`.

## Build and run

You need a C compiler (GCC or Clang) and Make. From the project root:

```bash
make examples/arithmetic.pol
```

This builds the interpreter when needed and runs the example. Any `.pol` file in `examples/` can be run this way.

See [examples/README.md](examples/README.md) for the available programs and expected results.

To build the interpreter and run your own file:

```bash
make
./build/polisz path/to/program.pol
```

Run `make clean` to remove the executable and object files.

## Language basics

| Syntax | Meaning |
| --- | --- |
| `name = expr;` | Create or update a variable without a declaration. |
| `+`, `-`, `*`, `/` | Integer arithmetic with standard precedence. Use parentheses to group expressions. |
| `-expr`, `+expr` | Unary minus and plus. |
| `wyswietl(expr);` | Print an integer followed by a newline. |
| `podaj(name);` | Read an integer into an existing or new variable. |
| `jezeli(expr) { ... }` | Execute a block when the expression is non-zero. Zero is false; negative values are also true. |
| `// ...` | Comment until the end of the line. |

Assignments and I/O statements end with `;`. Blocks use braces and can be nested.

Division truncates toward zero: `7 / 2` is `3`, and `-7 / 2` is `-3`.

## Project structure

```text
src/          C source files and headers
examples/     Example programs and expected results
build/        Generated executable and object files (ignored by Git)
Makefile      Build and run commands
README.md     Project documentation
```

The code is split into a lexer, a recursive-descent parser, and a tree-walk interpreter.

## Limitations

- Values are C `int` integers; strings and floating-point numbers are not supported.
- There are no comparison or logical operators, `else`, loops, or user-defined functions.
- Variables share a single global scope, including inside blocks.
- Identifiers must start with a letter and contain only letters and digits; underscores are not supported.
- Errors stop execution, and diagnostics do not include source line numbers.

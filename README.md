# Polish Programming Language Interpreter

A lightweight, tree-walk interpreter written in C for a custom programming language featuring Polish keywords. This project demonstrates the core concepts of language implementation, including lexical analysis, parsing into an Abstract Syntax Tree (AST), and program execution through an environment-based interpreter.

## Features

* **Custom Syntax**: Programming with Polish keywords (`wyswietl`, `podaj`, `jezeli`).
* **Modular Architecture**: Clean separation of concerns between the Lexer, Parser, and Interpreter.
* **AST Implementation**: Uses an Abstract Syntax Tree to represent the program structure before execution.
* **Variable Management**: Dynamic variable assignment and storage within a managed environment.
* **Arithmetic Operations**: Supports basic integer math (`+`, `-`, `*`, `/`).
* **I/O Operations**: Built-in functions for console output and user input.

## Project Structure

```text
src/          C source files and headers
examples/     Example programs (.pol)
build/        Generated executable and object files (ignored by Git)
Makefile      Build configuration
README.md     Project documentation
```

The source code in `src/` is organized into modular components:

* **`lexer.c` / `lexer.h`**: Performs lexical analysis, converting raw source code into a stream of tokens (Tokenization).
* **`parser.c` / `parser.h`**: Analyzes the token stream according to the language grammar and builds the Abstract Syntax Tree (AST).
* **`interpreter.c` / `interpreter.h`**: Recursively traverses the AST to execute the program's logic and manage the runtime environment.
* **`main.c`**: The entry point that handles file I/O and orchestrates the interpretation pipeline.

## Language Syntax

The language uses a C-like syntax with specific keywords:

| Keyword / Symbol | Description | Example |
|:---|:---|:---|
| `wyswietl(expr);` | Prints the value of an expression to the console. | `wyswietl(a + 5);` |
| `podaj(var);` | Prompts the user to input an integer value for a variable. | `podaj(x);` |
| `jezeli(cond) { ... }` | Executes a block of code if the condition is true (non-zero). | `jezeli(x) { wyswietl(1); }` |
| `=` | Assigns a value to a variable. | `count = 10;` |

### Example Program

```plaintext
a = 5;
b = 3;
wyswietl(a + b);  // Output: 8
podaj(c);         // User inputs a value, e.g., 7
wyswietl(c);      // Output: 7
```

## Getting Started

### Prerequisites

* **GCC Compiler** (or any standard C compiler)
* **Make** (optional, if you want to automate builds)

### Compilation

From the project root, run:

```bash
make
```

The executable is created at `build/polisz`. To remove the generated files, run `make clean`.

You can also compile without Make:

```bash
mkdir -p build
gcc -Wall -Wextra -g -o build/polisz src/main.c src/lexer.c src/parser.c src/interpreter.c
```

### Usage

Build the interpreter and run an example in one command:

```bash
make examples/arithmetic.pol
```

Any `.pol` file in `examples/` can be run this way. The interpreter is rebuilt when needed, and the example runs every time.

See [examples/README.md](examples/README.md) for the available programs and expected results.

You can also run the interpreter directly with a source file (`.pol`) as an argument:

```bash
./build/polisz examples/arithmetic.pol
```

## How It Works

1. **Source Code**: The user provides a `.pol` file.
2. **Lexer**: Scans the file and generates tokens (e.g., `TOKEN_PRINT`, `TOKEN_NUMBER`).
3. **Parser**: Consumes tokens to create nodes (e.g., `ASTNode` for assignments or if-statements).
4. **Interpreter**: Traverses the nodes, evaluates expressions (handling precedence and variables), and performs actions.

## License

This project is open-source and available for educational purposes.

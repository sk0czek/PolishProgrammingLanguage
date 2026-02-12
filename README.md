# Polish Programming Language Interpreter

A lightweight, tree-walk interpreter written in C for a custom programming language featuring Polish keywords. This project demonstrates the core concepts of language implementation, including lexical analysis, parsing into an Abstract Syntax Tree (AST), and program execution through an environment-based interpreter.

## 🚀 Features

* **Custom Syntax**: Programming with intuitive keywords mixed with Polish vocabulary (`wyswietl`, `podaj`) and standard control structures (`if`).
* **Modular Architecture**: Clean separation of concerns between the Lexer, Parser, and Interpreter.
* **AST Implementation**: Uses an Abstract Syntax Tree to represent the program structure before execution.
* **Variable Management**: Dynamic variable assignment and storage within a managed environment.
* **Arithmetic Operations**: Supports basic integer math (`+`, `-`, `*`, `/`).
* **I/O Operations**: Built-in functions for console output and user input.

## 🛠️ Project Structure

The project is organized into modular components:

* **`lexer.c` / `lexer.h`**: Performs lexical analysis, converting raw source code into a stream of tokens (Tokenization).
* **`parser.c` / `parser.h`**: Analyzes the token stream according to the language grammar and builds the Abstract Syntax Tree (AST).
* **`interpreter.c` / `interpreter.h`**: Recursively traverses the AST to execute the program's logic and manage the runtime environment.
* **`main.c`**: The entry point that handles file I/O and orchestrates the interpretation pipeline.

## 📝 Language Syntax

The language uses a C-like syntax with specific keywords:

| Keyword / Symbol | Description | Example |
|:---|:---|:---|
| `wyswietl(expr);` | Prints the value of an expression to the console. | `wyswietl(a + 5);` |
| `podaj(var);` | Prompts the user to input an integer value for a variable. | `podaj(x);` |
| `if(cond) { ... }` | Executes a block of code if the condition is true (non-zero). | `if(x) { wyswietl(1); }` |
| `=` | Assigns a value to a variable. | `count = 10;` |

### Example Program (`test.pol`)

```plaintext
a = 5;
b = 3;
wyswietl(a + b);  // Output: 8
podaj(c);         // User inputs a value, e.g., 7
wyswietl(c);      // Output: 7
```

## 📦 Getting Started

### Prerequisites

* **GCC Compiler** (or any standard C compiler)
* **Make** (optional, if you want to automate builds)

### Compilation

To compile the interpreter, use the following command in your terminal:

```bash
gcc -o interpreter main.c lexer.c parser.c interpreter.c
```

### Usage

Run the interpreter by passing the path to your source file (`.pol`) as an argument:

```bash
./interpreter prosty.pol
```

## 🔍 How It Works

1. **Source Code**: The user provides a `.pol` file.
2. **Lexer**: Scans the file and generates tokens (e.g., `TOKEN_PRINT`, `TOKEN_NUMBER`).
3. **Parser**: Consumes tokens to create nodes (e.g., `ASTNode` for assignments or if-statements).
4. **Interpreter**: Traverses the nodes, evaluates expressions (handling precedence and variables), and performs actions.

## 📄 License

This project is open-source and available for educational purposes.

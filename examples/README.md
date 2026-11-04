# Examples

Run an example from the project root:

```bash
make examples/arithmetic.pol
```

| Program | Demonstrates | Printed values |
| --- | --- | --- |
| [variables.pol](variables.pol) | Assignment, copying a value, and updating a variable. | `15`, `10` |
| [arithmetic.pol](arithmetic.pol) | All four operators, precedence, parentheses, unary signs, and integer division. | `14`, `20`, `12`, `2`, `3`, `-3`, `-3`, `-5`, `3` |
| [input.pol](input.pol) | Reading into an existing variable and creating a new one with `podaj`. | Enter `3` and `4` to print `7` and `12`. |
| [conditionals.pol](conditionals.pol) | Non-zero and zero conditions, nested blocks, and assignments inside a block. | `3`, `2`, `1`, `2` |

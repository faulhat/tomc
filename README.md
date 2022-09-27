# tomc

`tomc` is short for Tom's C Compiler. I am Tom.

The idea here is to build a C compiler that uses [asmjit](https://github.com/asmjit/asmjit), so that it can be used in REPL form.

The project can be built on Linux using the command:

`cmake . && make`

At this point, the program will parse C code and output a DOT file representing the AST. You can use this tool like this (after building the executable):

```
bin/tomc resources/hello.c.txt > out/tree.dot
dot out/tree.dot -Tpng > out/tree.png
```

After which `tree.png` should look like this:

![abstract syntax tree](/demo/tree.png)

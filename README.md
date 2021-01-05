# Lud-Lang
High-level programming language for embedded systems inspired by languages Python, Java and C.

## Compiler
Built implementing a LL(*) parser and using visitor pattern for modules type checker, optimizer, code generator and static linker.

## Compiler specifications
The keyword 'lud' is used to start the compiler.

The compiler is server based, meaning that there is only "one" compiler running on a server. This program sends the input file after which an executable file will be sent back.
To compile a program, simply write 'lud /<PATH>/<FILENAME>'. It is also possible to compile locally. To do this, write 'lud /<PATH>/<FILENAME> -off', but the version of the offline compiler might is up to you to update.

Parameter '-update' must be used seperately to updates the offline compiler. Parameter '-c' makes the compiler output only the target language (C++). To name the compiled file, use the '-o' flag followed by the desired name.

## Lud-Lang program rules
Every program must contain a class containing a 'main' method declared as 'public static main(string[] args) : void'. The name of this class must be the same as the file to be compiled.

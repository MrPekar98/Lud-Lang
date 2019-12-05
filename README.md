# Lud-Lang
High-level programming language for embedded systems inspired by languages Python, Java and C.

## Compiler
The compiler makes use of an LL(*) parser for better error messaging.

## Compiler specifications
The keyword 'lud' is used to start the compiler.

The compiler is server based, meaning that there is only one compiler running on a server. This program sends the input file after which an executable file in compiled language will be sent back to the client.
To compile a program, simply write 'lud /Directory/program.lud'. It is also possible to compile locally in case internet connection isn't available. For this, write 'lud /DIRECTORY/PROGRAM.lud -off', but the version of the offline compiler might not be up to date.

Parameter '-update' must be used seperately and updates the offline compiler. Parameter '-c' makes the compiler only output the target language. To name the compiled file, write 'lud /DIRECTORY/PROGRAM.lud -o NAME'.

## Lud-Lang program rules
Every program must contain a class containing a 'main' method declared as 'public static main(string[] args) : void'. The name of this class must be the same as the file to be compiled.

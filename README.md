# Lud-Lang
Python and Java inspired object-oriented programming language.

## Compiler
The compiler makes use of an LL(*) parser for better error messaging.

## Compiler specifications
The keyword 'lud' is used to start the compiler.

The compiler is server based, meaning that there is only one compiler running on a server. This program sends the input file after which an executable file in compiled language will be sent back to the client.
To compile a program, simply write 'lud /Directory/program.lud'. It is also possible to compile locally in case internet connection isn't available. For this, write 'lud /DIRECTORY/PROGRAM.lud -off'.

Parameter '-update' must be used seperately and updates the offline compiler.

All executable files will be named 'a'. To name the file before compilation, write 'lud /DIRECTORY/PROGRAM.lud -o NAME'.

## Lud-Lang program rules
Every program must contain a class containing a 'main' method declared as 'public static main(string[] args) : void'.

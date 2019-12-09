# CSC-502 - Graduate-Project

The idea of this project is to implement an Interpreter for Cuppa1 language - one of the languages that we have covered in class and implement it using lex (ﬂex) and yacc (bison) under Linux. The implementation language used is C. This project has been done on a Linux system - the virtual machine provided with the class 

Running the Interpreter:
$ yacc -d cuppa1.y

$ lex cuppa1.l

$ gcc y.tab.c -lm lex.yy.c

$ ./a.out < input_file.txt

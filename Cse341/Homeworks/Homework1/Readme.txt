To compile and run flex :
$ flex gpp_lexer.l && gcc lex.yy.c -o run && ./run

To compile and run lisp :
$ clisp gpp_lexer.lisp


! Both codes will wait for user input and work until you force quit (Ctrl+c)
! I waited for force quit because "exit" is a keyword in g language
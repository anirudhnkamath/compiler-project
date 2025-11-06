bison -dy opt.y
lex opt.l
gcc lex.yy.c y.tab.c opt.c
./a.out

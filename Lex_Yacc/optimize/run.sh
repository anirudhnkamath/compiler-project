bison -dy optimize.y
lex optimize.l
gcc lex.yy.c y.tab.c optimize.c
./a.out

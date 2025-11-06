set -e

bison -dy yacc.y
lex lex.l
gcc lex.yy.c y.tab.c ./../Parse_Tree/parse_tree.c ./../Symbol_Table/global_symbol_table.c ./../Type_Table/type_table.c ./../Three_addr_code_gen/Tac_gen.c -o final
./final ./../Sample_input/input_1.txt
cp targetFile.txt ./optimize/targetFile.txt

cd optimize

./run.sh

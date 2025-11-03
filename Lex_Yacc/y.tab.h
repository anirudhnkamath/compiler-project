/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     NUM = 259,
     ADD = 260,
     SUB = 261,
     STAR = 262,
     DIV = 263,
     MOD = 264,
     T_BEGIN = 265,
     T_END = 266,
     READ = 267,
     WRITE = 268,
     ASSGN = 269,
     EOL = 270,
     IF = 271,
     THEN = 272,
     ELSE = 273,
     ENDIF = 274,
     GE = 275,
     LE = 276,
     EQ = 277,
     GT = 278,
     LT = 279,
     NE = 280,
     WHILE = 281,
     DO = 282,
     END_WHILE = 283,
     REPEAT = 284,
     UNTILL = 285,
     DECL = 286,
     ENDDECL = 287,
     INT = 288,
     OR = 289,
     AND = 290
   };
#endif
/* Tokens.  */
#define ID 258
#define NUM 259
#define ADD 260
#define SUB 261
#define STAR 262
#define DIV 263
#define MOD 264
#define T_BEGIN 265
#define T_END 266
#define READ 267
#define WRITE 268
#define ASSGN 269
#define EOL 270
#define IF 271
#define THEN 272
#define ELSE 273
#define ENDIF 274
#define GE 275
#define LE 276
#define EQ 277
#define GT 278
#define LT 279
#define NE 280
#define WHILE 281
#define DO 282
#define END_WHILE 283
#define REPEAT 284
#define UNTILL 285
#define DECL 286
#define ENDDECL 287
#define INT 288
#define OR 289
#define AND 290




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 11 "yacc.y"
{
    ast_node* AST_node;
}
/* Line 1529 of yacc.c.  */
#line 123 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;


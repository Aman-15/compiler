/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#ifndef LEXER_DEF_H
#define LEXER_DEF_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "parserDef.h"

#define BUFFER_SIZE 1000

enum enum_token {
    DECLARE, DRIVER, PROGRAM, FOR, START, END, MODULE, GET_VALUE, PRINT, USE, WITH, PARAMETERS, TRUE, FALSE,
    TAKES, INPUT, RETURNS, AND, OR, SWITCH, CASE, BREAK, DEFAULT, WHILE, INTEGER, REAL, BOOLEAN, OF, ARRAY, 
    IN, ID, RNUM, NUM, COLON, ASSIGNOP, COMMA, NE, RANGEOP, GT, ENDDEF, GE, SEMICOL, EQ, MINUS, LT, DEF,
    LE, PLUS, MUL, COMMENTMARK, DIV, SQBO, SQBC, BO, BC, DRIVERDEF, DRIVERENDDEF, $, e, program=0, moduleDeclarations, 
    otherModules, driverModule, moduleDeclaration, module, moduleDef, input_plist, ret, output_plist, dataType, 
    input_plist2, type, output_plist2, range, statements, statement, ioStmt, simpleStmt, declareStmt, conditionalStmt, 
    iterativeStmt, var, whichId, assignmentStmt, moduleReuseStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, expression, 
    index1, optional, idList, idList2, arithmeticOrBooleanExpr, arithmeticExpr, anyTerm, arithmeticOrBooleanExpr2, 
    logicalOp, anyTerm2, relationalOp, term, arithmeticExpr2, op1, factor, term2, op2, caseStmts, default1, value, 
    booleanOrNonBooleanArithmeticExpr, expressionWithLogOp, expressionWithRelOp, 
    negOrPosArithmeticExpr, caseStmt
};

union value {
    int int_val;
    float float_val;
};

struct token {
    enum enum_token tokenID;
    char lexeme[20];
    union value val;
    struct token *prev, *next;
    int line_num;
};

#endif
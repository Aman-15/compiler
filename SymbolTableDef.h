/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/

#ifndef SYMBOLTABLE_DEF_H
#define SYNMOLTABLE_DEF_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"

enum FuncStatus {
    declared, defined, none
};

struct IdTuple {
    char name[20];
    int line_dec, line_ref;
    enum enum_token type;
    int arr_or_not;
    int start, end;
    int width, offset;
    struct IdTuple *next;
};

struct IdTable {
    char iterator[10];
    int nesting;
    int beg, last;
    int count;
    struct IdTable *child, *next, *parent;
    struct IdTuple *idTuple, *idTupleLast;
};

struct Parameters {
    char name[20];
    enum enum_token type;
    int line_num;
    int arr_or_not;
    int start, end;
    int width, offset;
    int assigned;
    struct Parameters *next;
};

struct FuncTable {
    char name[20];
    struct Parameters *out, *in;
    enum FuncStatus status;
    struct IdTable *idTable;
    struct FuncTable *next;
};

struct tnllnode {
	TREENODEPTR node;
	struct tnllnode *next;
};

struct tnll {
	int count;
	struct tnllnode *first;
};

#endif
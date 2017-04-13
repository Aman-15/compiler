/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/

#ifndef SYMBOLTABLE_H
#define SYNMOLTABLE_H
#include "SymbolTableDef.h"
#include "parser.h"

struct FuncTable *funcList;
struct IdTable *global;

TREENODEPTR top_st(struct tnll *stack);
void push_st(struct tnll *stack, TREENODEPTR n);
void pop_st(struct tnll *stack);
int isEmpty(struct tnll *stack);

struct FuncTable* findInFunctionList(char *name);
int checkIdentifierifDeclared(struct IdTable *table, char *name);
int checkinTable(struct IdTable *table, char *name);
int checkInParameters(struct FuncTable *curr, char *name);

struct FuncTable* addFunc(struct FuncTable* currFuncTable);
void addInParameter(struct FuncTable *currFuncTable, struct Parameters *inp, struct Parameters *outp);
struct IdTable* getNewTable();
struct IdTable* addNestedScope(struct IdTable *currIdTable);
void addTupleEntry(struct IdTable *currIdTable, char *name, int line_dec, int line_ref, enum enum_token type, int arr_or_not, int start, int end);

void makeTable(TREENODEPTR treeNode);

#endif
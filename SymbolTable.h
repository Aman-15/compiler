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

TREENODEPTR top(struct tnll *stack);
void push(struct tnll *stack, TREENODEPTR n);
void pop(struct tnll *stack);
int isEmpty(struct tnll *stack);

struct FuncTable* findInFunctionList(char *name);
int checkParameters(struct FuncTable *func, struct Parameters *out, struct Parameters *in);
int checkIdentifierifDeclared(struct IdTable *table, char *name);
int checkinTable(struct IdTable *table, char *name);

struct FuncTable* addFunc(struct FuncTable* currFuncTable);
void addInParameter(struct FuncTable *currFuncTable, struct Parameters *params);
struct IdTable* getNewTable();
struct IdTable* addNestedScope(struct IdTable *currIdTable);
void addTupleEntry(struct IdTable *currIdTable, char *name, int line_dec, int line_ref, enum enum_token type);

void makeTable(TREENODEPTR treeNode);

#endif
/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexerDef.h"

void appendSymbol(int id);
void addKeyword(int id, char *keyword);
int isKeyword(char *word);
void addID(char *id);
void addNUM(int n_or_r, char *num, float value);
void matchSymbols(char *buf);
FILE* getStream(FILE *fp, char *buf);
void generateList(char *file);
struct token* getNextToken();
void printlist();
void removeComments(char *testcaseFile, char *cleanFile);
void removeCommentsConsole(char *testcaseFile);
struct token* getFirst();
struct token *returnHead();

#endif
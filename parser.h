/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parserDef.h"

NODEPTR top(LISTPTR stack);
void push(LISTPTR stack, enum enum_token t, int t_or_nt);
void pop(LISTPTR stack);
void insert(LISTPTR ls, enum enum_token t, int t_or_nt);
void populateTable();
void generateFollowSet(char *s);
void generateFirstSet(char *s);
void inputGrammar(char *gramFile);
int *parseInputSourceCode(struct token* tokens);
void printTreeInorder(FILE *fp, TREENODEPTR tree);
TREENODEPTR generateParseTree(int *ruleList, int *index, int len, TREENODEPTR parent);

#endif
/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#ifndef PARSER_DEF_H
#define PARSER_DEF_H

#include <stdio.h>
#include <string.h>
#include "lexerDef.h"

#define count_nonTerminal caseStmt+1
#define count_terminal e+1
#define count_rule 107

struct node {
	enum enum_token t;
    int t_or_nt;
	struct node *next;
};
typedef struct node *NODEPTR;

struct linkedlist {
	int count;
	NODEPTR first;
};
typedef struct linkedlist *LISTPTR;

struct parseTreeNode {
    enum enum_token t;
    int t_or_nt;
    char lexeme[20];
    union value val;
    int line_num;
    struct parseTreeNode **child;
    struct parseTreeNode *parent;
    int child_count;
    struct IdTuple  *tuple;
};
typedef struct parseTreeNode *TREENODEPTR;

#endif
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
#define count_rule 94

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
    struct parseTreeNode **child;
    struct parseTreeNode *parent;
    int child_count;
};
typedef struct parseTreeNode *TREENODEPTR;

#endif
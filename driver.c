/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include <stdio.h>
#include <malloc.h>
#include "lexer.h"
#include "parser.h"
#include "SymbolTable.h"

int main(int argc, char *argv[])
{
	inputGrammar("grammar_v2.txt");
	generateFirstSet("FirstSet_v2.txt");
	generateFollowSet("FollowSet_v2.txt");
	printf("FIRST and FOLLOW set automated\n");
	generateList(argv[1]);
	populateTable();
	printf("both lexical and syntax analysis modules implemented\n");
	struct token *head = returnHead();
	int *rules;
	int op;
	scanf("%d", &op);
	int j = 0;
	int i = 0;
	FILE *fp;
	TREENODEPTR root;
	switch(op)
	{
		case 1:
			removeCommentsConsole(argv[1]);
			break;
		case 2:
			printlist();
			break;
		case 3:
			rules = parseInputSourceCode(head->next);
			break;
		case 4:
			rules = parseInputSourceCode(head->next);
			for(i = 0; rules[i] != -1; i++);
			
			root = generateParseTree(rules, &j, i, NULL);
			setFields(root, &(returnHead()->next));
			fp = fopen(argv[2], "w");
			printTreeInorder(fp, root);
			printf("Parse Tree Printed\n");
			// printf("%s\n", string_tokens1[root->t + count_terminal]);
			makeTable(root);
			break;
	}
	return 0;
}

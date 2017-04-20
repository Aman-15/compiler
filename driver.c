/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include <stdio.h>
#include <malloc.h>
#include "lexer.h"
#include "parser.h"
// #include "SymbolTable.h"
#include "AST.h"
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

	printf("1. Print the Token list\n");
	printf("2. Parse the code to verify syntactic correctness and print Parse Tree\n");
	printf("3. Print the Abstract Syntax Tree (Pre Order)\n");
	printf("4. Display the memory allocated to the Parse tree and AST\n");
	printf("5. Print the Symbol Table\n");
	printf("6. Compile the code to verify syntactic and semantic correctness\n");
	printf("7. Produce the assembly code\n");
	printf("\nEnter choice. ");	

	scanf("%d", &op);
	int j = 0;
	int i = 0;
	// FILE *fp = NULL;
	TREENODEPTR root;
	struct ASTNode *ast_root;
	switch(op)
	{
		case 1:
			printlist();
			break;
		
		case 2:
			rules = parseInputSourceCode(head->next);
			if (rules == NULL) {
				printf("Syntax Error\n");
				return 0;
			}
			for(i = 0; rules[i] != -1; i++);
			root = generateParseTree(rules, &j, i, NULL);
			printf("PARSE TREE generated\n");
			setFields(root, &(returnHead()->next));
			printTreeInorder(root);
			break;
		
		case 3:
			rules = parseInputSourceCode(head->next);
			if (rules == NULL) {
				printf("Syntax Error\n");
				return 0;
			}
			for(i = 0; rules[i] != -1; i++);
			root = generateParseTree(rules, &j, i, NULL);
			printf("PARSE TREE generated\n");
			setFields(root, &(returnHead()->next));
			makeTable(root);
			ast_root = generateAST(root);
			printf("AST generated\n");
			printAST(ast_root);
			break;

		case 4:
			rules = parseInputSourceCode(head->next);
			if (rules == NULL) {
				printf("Syntax Error\n");
				return 0;
			}
			for(i = 0; rules[i] != -1; i++);
			root = generateParseTree(rules, &j, i, NULL);
			printf("PARSE TREE generated\n");
			setFields(root, &(returnHead()->next));
			makeTable(root);
			ast_root = generateAST(root);
			printf("AST generated\n");
			//displayAmountOfMemory();
			break;

		case 5:
			rules = parseInputSourceCode(head->next);
			if (rules == NULL) {
				printf("Syntax Error\n");
				return 0;
			}
			for(i = 0; rules[i] != -1; i++);
			root = generateParseTree(rules, &j, i, NULL);
			printf("PARSE TREE generated\n");
			setFields(root, &(returnHead()->next));
			makeTable(root);
			printf("Symbol Table generated\n");
			printSymbolTable();			
			break;

		case 6:
			rules = parseInputSourceCode(head->next);
			if (rules == NULL) {
				printf("Syntax Error\n");
				return 0;
			}
			for(i = 0; rules[i] != -1; i++);
			root = generateParseTree(rules, &j, i, NULL);
			printf("PARSE TREE generated\n");
			setFields(root, &(returnHead()->next));
			makeTable(root);
			checkIfOutAssigned();
			// printf("here\n");
			ast_root = generateAST(root);
			//typeCheck();
			break;

		case 7:
			rules = parseInputSourceCode(head->next);
			if (rules == NULL) {
				printf("Syntax Error\n");
				return 0;
			}
			for(i = 0; rules[i] != -1; i++);
			root = generateParseTree(rules, &j, i, NULL);
			printf("PARSE TREE generated\n");
			setFields(root, &(returnHead()->next));
			makeTable(root);
			ast_root = generateAST(root);
			//typeCheck();
			FILE *fp = fopen("code.asm", "w");
			//printAssemblyCode();

		default:
			printf("Wrong Choice.\n");
			printf("Exiting...\n");
			break;
	}
	return 0;
}

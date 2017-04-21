/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include <stdio.h>
#include <malloc.h>
#include "lexer.h"
#include "codegen.h"

int main(int argc, char *argv[]) {
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
	printf("\nLEVEL 4: Symbol Table / AST / Type Checking / Semantic Rules Modules Work\n");
	printf("\n1. Print the Token list\n");
	printf("2. Parse the code to verify syntactic correctness and print Parse Tree\n");
	printf("3. Print the Abstract Syntax Tree (Pre Order)\n");
	printf("4. Display the memory allocated to the Parse tree and AST\n");
	printf("5. Print the Symbol Table\n");
	printf("6. Compile the code to verify syntactic and semantic correctness\n");
	printf("7. Produce the assembly code\n");
	printf("\nEnter choice. ");	


	scanf("%d", &op);
	int j;
	int i;
	int root_generated = 0, fieldsset = 0, tablemade = 0, astgenerated = 0, rulesmade = 0; 
	TREENODEPTR root;
	struct ASTNode *ast_root;

	
	// while (op >= 1 && op <= 7) {
		switch(op) {
			case 1:
				printlist();
				break;
			
			case 2:
				i = j = 0;
				if (!rulesmade) {
					rules = parseInputSourceCode(head->next);
					rulesmade = 1;
				}
				if (rules == NULL) {
					printf("Syntax Error\n");
					return 0;
				}
				for(i = 0; rules[i] != -1; i++);

				if (!root_generated) {
					root = generateParseTree(rules, &j, i, NULL);
					root_generated = 1;
				}
				if (!fieldsset) {
					setFields(root, &(returnHead()->next));
					fieldsset = 1;
				}
				printTreeInorder(root, 1);
				break;
			
			case 3:
				i = j = 0;
				if (!rulesmade) {
					rules = parseInputSourceCode(head->next);
					rulesmade = 1;
				}
				if (rules == NULL) {
					printf("Syntax Error\n");
					return 0;
				}
				for(i = 0; rules[i] != -1; i++);
				if (!root_generated) {
					root = generateParseTree(rules, &j, i, NULL);
					root_generated = 1;
				}
				if (!fieldsset) {
					setFields(root, &(returnHead()->next));
					fieldsset = 1;
				}
				if (!tablemade) {
					makeTable(root);
					tablemade = 1;
				}
				if (!astgenerated) {
					ast_root = generateAST(root);
					astgenerated = 1;
				}
				printAST(ast_root, 1);
				break;

			case 4:
				i = j = 0;
				if (!rulesmade) {
					rules = parseInputSourceCode(head->next);
					rulesmade = 1;
				}
				if (rules == NULL) {
					printf("Syntax Error\n");
					return 0;
				}
				for(i = 0; rules[i] != -1; i++);
				if (!root_generated) {
					root = generateParseTree(rules, &j, i, NULL);
					root_generated = 1;
				}
				if (!fieldsset) {
					setFields(root, &(returnHead()->next));
					fieldsset = 1;
				}
				if (!tablemade) {
					makeTable(root);
					tablemade = 1;
				}
				if (!astgenerated) {
					ast_root = generateAST(root);
					astgenerated = 1;
				}
				printAST(ast_root, 0);
				printTreeInorder(root, 0);
				printf("Parse Tree Number of nodes = %d Allocated Memory = %d Bytes\n", getNoOfParsTNodes(), getSizeOfParsT());
				printf("AST Number of nodes = %d Allocated Memory = %d Bytes\n", getNoOfASTNodes(), getSizeOfAST());
				printf("Compression Percentage = %.2lf\n", ((double)(getSizeOfParsT() - getSizeOfAST()) * 100) / (double)getSizeOfParsT() );
				break;

			case 5:
				i = j = 0;
				if (!rulesmade) {
					rules = parseInputSourceCode(head->next);
					rulesmade = 1;
				}
				if (rules == NULL) {
					printf("Syntax Error\n");
					return 0;
				}
				for(i = 0; rules[i] != -1; i++);
				if (!root_generated) {
					root = generateParseTree(rules, &j, i, NULL);
					root_generated = 1;
				}
				if (!fieldsset) {
					setFields(root, &(returnHead()->next));
					fieldsset = 1;
				}
				if (!tablemade) {
					makeTable(root);
					tablemade = 1;
				}
				printSymbolTable();			
				break;

			case 6:
				i = j = 0;
				if (!rulesmade) {
					rules = parseInputSourceCode(head->next);
					rulesmade = 1;
				}
				if (rules == NULL) {
					printf("Syntax Error\n");
					return 0;
				}
				for(i = 0; rules[i] != -1; i++);
				if (!root_generated) {
					root = generateParseTree(rules, &j, i, NULL);
					root_generated = 1;
				}
				if (!fieldsset) {
					setFields(root, &(returnHead()->next));
					fieldsset = 1;
				}
				if (!tablemade) {
					makeTable(root);
					tablemade = 1;
				}
				checkIfOutAssigned();
				if (!astgenerated) {
					ast_root = generateAST(root);
					astgenerated = 1;
				}
				if (getErrST() == 0 && getErrAst() == 0)
					printf("Code Compiled Successfully......\n");
				else printf("Compilation Error........\n");
				break;

			case 7:
				i = j = 0;
				if (!rulesmade) {
					rules = parseInputSourceCode(head->next);
					rulesmade = 1;
				}
				if (rules == NULL) {
					printf("Syntax Error\n");
					return 0;
				}
				for(i = 0; rules[i] != -1; i++);
				if (!root_generated) {
					root = generateParseTree(rules, &j, i, NULL);
					root_generated = 1;
				}
				if (!fieldsset) {
					setFields(root, &(returnHead()->next));
					fieldsset = 1;
				}
				if (!tablemade) {
					makeTable(root);
					tablemade = 1;
				}
				checkIfOutAssigned();
				if (!astgenerated) {
					ast_root = generateAST(root);
					astgenerated = 1;
				}
				if (getErrST() || getErrAst())
					printf("Compilation Error......\n");
				
				else {
					printAssemblyCode(root);
					printf("Assembly Code Generated......\n");
				}
				break;
			default:
				printf("Wrong Choice.\n");
				printf("Exiting...\n");
				break;
		}
		// printf("\nLEVEL 4: Symbol Table / AST / Type Checking / Semantic Rules Modules Work\n");
		// printf("\n1. Print the Token list\n");
		// printf("2. Parse the code to verify syntactic correctness and print Parse Tree\n");
		// printf("3. Print the Abstract Syntax Tree (Pre Order)\n");
		// printf("4. Display the memory allocated to the Parse tree and AST\n");
		// printf("5. Print the Symbol Table\n");
		// printf("6. Compile the code to verify syntactic and semantic correctness\n");
		// printf("7. Produce the assembly code\n");
		// printf("\nEnter choice. ");	

		// scanf("%d", &op);
	// }
	return 0;
}

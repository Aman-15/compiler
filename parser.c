/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
//#include "forstring.h"
char *string_tokens[] = {
    "DECLARE\0", "DRIVER\0", "PROGRAM\0", "FOR\0", "START\0", "END\0", "MODULE\0", "GET_VALUE\0", "PRINT\0", 
    "USE\0", "WITH\0", "PARAMETERS\0", "TRUE\0", "FALSE\0", "TAKES\0", "INPUT\0", "RETURNS\0", "AND\0", "OR\0", 
    "SWITCH\0", "CASE\0", "BREAK\0", "DEFAULT\0", "WHILE\0", "INTEGER\0", "REAL\0", "BOOLEAN\0", "OF\0", "ARRAY\0", 
    "IN\0", "ID\0", "RNUM\0", "NUM\0", "COLON\0", "ASSIGNOP\0", "COMMA\0", "NE\0", "RANGEOP\0", "GT\0", "ENDDEF\0", 
    "GE\0", "SEMICOL\0", "EQ\0", "MINUS\0", "LT\0", "DEF\0", "LE\0", "PLUS\0", "MUL\0", "COMMENTMARK\0", "DIV\0", 
    "SQBO\0", "SQBC\0", "BO\0", "BC\0", "DRIVERDEF\0", "DRIVERENDDEF\0", "$\0", "e\0", "program\0", 
    "moduleDeclarations\0", "otherModules\0", "driverModule\0", "moduleDeclaration\0", "module\0", "moduleDef\0", 
    "input_plist\0", "ret\0", "output_plist\0", "dataType\0", "input_plist2\0", "type\0", "output_plist2\0", 
    "range\0", "statements\0", "statement\0", "ioStmt\0", "simpleStmt\0", "declareStmt\0", "conditionalStmt\0", 
    "iterativeStmt\0", "var\0", "whichId\0", "assignmentStmt\0", "moduleReuseStmt\0", "whichStmt\0", "lvalueIDStmt\0", 
    "lvalueARRStmt\0", "expression\0", "index1\0", "optional\0", "idList\0", "idList2\0", "arithmeticOrBooleanExpr\0", 
    "arithmeticExpr\0", "anyTerm\0", "arithmeticOrBooleanExpr2\0", "logicalOp\0", "anyTerm2\0", "relationalOp\0", 
    "term\0", "arithmeticExpr2\0", "op1\0", "factor\0", "term2\0", "op2\0", "caseStmts\0", "default1\0", "value\0", 
    "booleanOrNonBooleanArithmeticExpr\0", "expressionWithLogOp\0", "expressionWithRelOp\0", "negOrPosArithmeticExpr\0", 
    "caseStmt\0"
};

LISTPTR *first, *follow, *grammar;
int parseTable[count_nonTerminal][count_terminal];

NODEPTR top(LISTPTR stack) //first element of the linked list
{
	return stack->first;
}

void push(LISTPTR stack, enum enum_token t, int t_or_nt)
{
	NODEPTR n = (NODEPTR)malloc(sizeof(struct node));
	n->t = t;
	n->t_or_nt = t_or_nt;
	n->next = NULL;
	NODEPTR temp = stack->first;
	stack->first = n;
	n->next = temp;
	stack->count++;
}

void pop(LISTPTR stack) {
	if(stack == NULL || stack->first == NULL)
	{
		printf("STACK IS EMPTY!! EXITING\n");
		return;
	}
	// printf("%s\n", string_tokens[top(stack)->t+top(stack)->t_or_nt*count_terminal]);
	NODEPTR temp = top(stack);
	stack->first = stack->first->next;
	temp->next = NULL;
	free(temp);
	stack->count--;
}

void insert(LISTPTR ls, enum enum_token t, int t_or_nt) {
	NODEPTR n = (NODEPTR)malloc(sizeof(struct node));
	n->t = t;
	n->t_or_nt = t_or_nt;
	n->next = NULL;
	if(ls->first == NULL)
		ls->first = n;
	else {
		NODEPTR temp = ls->first;
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = n;
	}
	ls->count++;
}

void populateTable() {
    int i;
    int j;
    for(i = 0; i < count_nonTerminal; i++)
    {
    	for(j = 0; j < count_terminal; j++) parseTable[i][j] = -1;
    }
    NODEPTR n1, n2;
    NODEPTR n;
	FILE *out = fopen("parseTable entries.txt", "w");
    for(i=0; i<count_rule; i++) {
        n = grammar[i]->first;
		// printf("%d\n", i);
        n1 = n->next;
        int flag;

        while (n1) {
            flag = 0;
            if (n1->t_or_nt) {
                if (n1->t != e) {
                    parseTable[n->t][n1->t] = i;
					fprintf(out, "1 inserted rule %d at %s,%s\n",i,string_tokens[n->t+count_terminal],string_tokens[n1->t]);
					break;
				}
                else {
                    //Follow part
                    n1 = follow[n->t]->first;
                    while (n1) {
                        parseTable[n->t][n1->t] = i;
						fprintf(out, "2 inserted rule %d at %s,%s\n",i,string_tokens[n->t+count_terminal],string_tokens[n1->t]);
                        n1 = n1->next;
                    }
                }
            }
            else {
                n2 = first[n1->t]->first;
                while (n2) {
                    if (n2->t != e) {
                        fprintf(out, "3 inserted rule %d at %s,%s\n",i,string_tokens[n->t+count_terminal],string_tokens[n2->t]);
                        parseTable[n->t][n2->t] = i;
					}
                    else
                        flag = 1;
                    n2 = n2->next;
                }
                if (flag)
                    n1 = n1->next;
                else
                    break;
            }
        }
        if (flag && n2 == NULL) {
            //Follow part
            n1 = follow[n->t]->first;
            while (n1) {
                parseTable[n->t][n1->t] = i;
                fprintf(out, "1 inserted rule %d at %s,%s\n",i,string_tokens[n->t+count_terminal],string_tokens[n1->t]);
                n1 = n1->next;
            }
        }
    }
    //printf("%d\n", parseTable[20][20]);
	fclose(out);
}


void generateFollowSet(char *FollowSetFile) {
	FILE *read = fopen(FollowSetFile, "r");
	char buf[201];
	int count = count_nonTerminal;
	follow = (LISTPTR *)malloc((count) * sizeof(LISTPTR));
	int r;
	for (r=0; r<count; r++) {
		fscanf(read, "%s", buf);
		int j=0;
		int enum_number;
		while (j < (count_terminal+count_nonTerminal)) {
			if (strcmp(string_tokens[j], buf)==0)
                break;
            j++;
        }
		enum_number = j-e-1;
		follow[enum_number] = (LISTPTR)malloc(sizeof(struct linkedlist));
        follow[enum_number]->count = 0;
        follow[enum_number]->first = NULL;
		int index = 0;
		fgets(buf, 201, read);
		while(buf[index] == ' ' || buf[index] == '\t')
			index++;
		int len = strlen(buf);
		char front = buf[index];
		while (index < len) {
			// int t_or_nt;
			// if (front >= 'A' && front <= 'Z' || (front == 'e' && buf[index+1] != 'x') || buf[index] == '$')
			// 	t_or_nt = 1;
			// else if (front >= 'a' && front <= 'z')
			// 	t_or_nt = 0;
			if ((front >= 'A' && front <= 'Z') || (front >= 'a' && front <= 'z') || buf[index] == '$') {
				char id[40];
				int i=0;
				id[i++] = front;
				index++;
				while ((buf[index]>='A' && buf[index]<='Z') || (buf[index]>='a' && buf[index]<='z') || buf[index]=='_' || ((buf[index]>='0' && buf[index]<='9') && buf[index]!='\n'))
					id[i++] = buf[index++];
				id[i] = '\0';
				int j=0;
				while (j < (count_terminal+count_nonTerminal)) {
					if (strcmp(string_tokens[j], id)==0) {
                        insert(follow[enum_number], j, 1);
                        break;
					}
                    j++;
                }
				index--;
			}
            front = buf[++index];
		}
	}
	fclose(read);
}

void generateFirstSet(char *FirstSetFile) {
	FILE *read = fopen(FirstSetFile, "r");
	char buf[201];
	int count = count_nonTerminal;
	first = (LISTPTR *)malloc((count) * sizeof(LISTPTR));
	int r;
	for (r=0; r<count; r++) {
		fscanf(read, "%s", buf);
		int j=0;
		int enum_number;
		while (j < (count_terminal+count_nonTerminal)) {
			if (strcmp(string_tokens[j], buf)==0)
                break;
            j++;
        }
		enum_number = j-e-1;
		first[enum_number] = (LISTPTR)malloc(sizeof(struct linkedlist));
        first[enum_number]->count = 0;
        first[enum_number]->first = NULL;
		int index = 0;
		fgets(buf, 201, read);
		while(buf[index] == ' ' || buf[index] == '\t')
			index++;
		int len = strlen(buf);
		char front = buf[index];
		while (index < len) {
			// int t_or_nt;
			// if (front >= 'A' && front <= 'Z' || (front == 'e' && buf[index+1] != 'x'))
			// 	t_or_nt = 1;
			// else if (front >= 'a' && front <= 'z')
			// 	t_or_nt = 0;
			if ((front >= 'A' && front <= 'Z') || (front >= 'a' && front <= 'z')) {
				char id[40];
				int i=0;
				id[i++] = front;
				index++;
				while ((buf[index]>='A' && buf[index]<='Z') || (buf[index]>='a' && buf[index]<='z') || buf[index]=='_' || ((buf[index]>='0' && buf[index]<='9') && buf[index]!='\n'))
					id[i++] = buf[index++];
				id[i] = '\0';
				int j=0;
				while (j < (count_terminal+count_nonTerminal)) {
					if (strcmp(string_tokens[j], id)==0) {
                        insert(first[enum_number], j, 1);
                        break;
					}
                    j++;
                }
				index--;
			}
            front = buf[++index];
		}
	}
	fclose(read);
}

void inputGrammar(char *gramFile) {
	FILE *read = fopen(gramFile, "r");
	char buf[201];
	int rules=count_rule;
	grammar = (LISTPTR *)malloc((rules) * sizeof(LISTPTR));
	int r;
	for (r=0; r<rules; r++) {
		grammar[r] = (LISTPTR)malloc(sizeof(struct linkedlist));
        grammar[r]->count = 0;
        grammar[r]->first = NULL;
		fgets(buf, 201, read);
		int index=0;
		int len = strlen(buf);
		char front = buf[index];
		while (index < len) {
			int t_or_nt;
			if ((front >= 'A' && front <= 'Z') || (front == 'e' && buf[index+1] != 'x'))
				t_or_nt = 1;
			else if (front >= 'a' && front <= 'z')
				t_or_nt = 0;
			if ((front >= 'A' && front <= 'Z') || (front >= 'a' && front <= 'z')) {
				char id[40];
				int i=0;
				id[i++] = front;
				index++;
				while ((buf[index]>='A' && buf[index]<='Z') || (buf[index]>='a' && buf[index]<='z') || buf[index]=='_' || ((buf[index]>='0' && buf[index]<='9') && buf[index]!='\n'))
					id[i++] = buf[index++];
				id[i] = '\0';
				int j=0;
				while (j < (count_terminal+count_nonTerminal)) {
					if (strcmp(string_tokens[j], id)==0) {
                        insert(grammar[r], t_or_nt ? j : j-e-1, t_or_nt);
                        break;
					}
                    j++;
                }
				index--;
			}
            front = buf[++index];
		}
	}
	fclose(read);
}

int *parseInputSourceCode(struct token* tokens) {
	int *rules = (int *)malloc(10000*sizeof(int));
	int i = 0; 
	for(i = 0; i < 10000; i++) rules[i] = -1;
	int index = 0;
	LISTPTR stack = (LISTPTR)malloc(sizeof(struct linkedlist));
	stack->count = 0;
	stack->first = NULL;
	struct token *tk = tokens;
	while(tk && tk->tokenID == COMMENTMARK) {
		tk = tk->next;
	}
	push(stack, $, 1);
	push(stack, program, 0);
	NODEPTR X = top(stack);
	int flag = 0;
	while(X->t != $)
	{
		if(X->t_or_nt == 1)
		{
			if(X->t == e)
				pop(stack);
			else if(X->t == tk->tokenID)
			{
				pop(stack);
				tk = tk->next;
				while(tk && tk->tokenID == COMMENTMARK) 
					tk = tk->next;
			}
			else {
				printf("line:%d The token \"%s\" for lexeme \"%s\" does not match. The expected token here is \"%s\"\n", tk->line_num, string_tokens[tk->tokenID],
					tk->lexeme, string_tokens[X->t]);
				flag = 1;

				while (tk && tk->tokenID != SEMICOL)
					tk = tk->next;
				while (tk && parseTable[X->t][tk->tokenID] == -1) {
					pop(stack);
					X = top(stack);
				}
			}
		}
		else
		{
			if(tk == NULL && stack->count > 1)
			{
				while(parseTable[top(stack)->t][$] != -1)
				{
					int r = parseTable[top(stack)->t][$];
					rules[index++] = r;
					pop(stack);
					if(top(stack)->t == $)
						break;
				}
				if(stack->count > 1) {
					printf("Stack not empty\n");
					//return NULL;
					break;
				}

			}
			else if(parseTable[X->t][tk->tokenID] == -1) {
				printf("line:%d Unexpected Token ", tk->line_num);
				printf("\"%s\"\n", tk->lexeme);
				flag = 1;
				
				while (tk && tk->tokenID != SEMICOL)
					tk = tk->next;
				while (tk && parseTable[X->t][tk->tokenID] == -1) {
					pop(stack);
					X = top(stack);
				}
			}
			else
			{
				int r = parseTable[X->t][tk->tokenID];
				rules[index++] = r; //add rule number for generation of parse tree
				pop(stack);
				NODEPTR rhs = grammar[r]->first->next;
				LISTPTR dummy_stack = (LISTPTR)malloc(sizeof(struct linkedlist));
				dummy_stack->count = 0;
				dummy_stack->first = NULL;
				while(rhs != NULL)
				{
					push(dummy_stack, rhs->t, rhs->t_or_nt);
					rhs = rhs->next;
				}
				while(dummy_stack->count > 0)
				{
					push(stack, top(dummy_stack)->t, top(dummy_stack)->t_or_nt);
					pop(dummy_stack);
				}	
			}
		}
		X = top(stack);

	}
	if(flag == 0 && tk != NULL) {// error
		printf("Error: token list exceeding\n");
		//return NULL;
	}
	else if (flag == 0) {
		printf("String Parsed Successfully\n");
	}
	else if (flag == 1)
		return NULL;
	return rules;
}

void printTreeInorder(TREENODEPTR tree) {
	if (tree != NULL) {
		if (tree->child_count > 0)
			printTreeInorder(tree->child[0]);
		
		if(tree->t_or_nt == 1) {
			if (tree->t == e)
				printf("---- ---- e ---- %s yes ----\n", string_tokens[tree->parent->t + count_terminal]);
			else {
				if (tree->t == NUM)
					printf("%s %d %s %d %s yes ----\n",tree->lexeme, tree->line_num, string_tokens[tree->t], 
						tree->val.int_val, string_tokens[tree->parent->t + count_terminal]);
				else if (tree->t == RNUM)
					printf("%s %d %s %f %s yes ----\n",tree->lexeme, tree->line_num, string_tokens[tree->t], 
						tree->val.float_val, string_tokens[tree->parent->t + count_terminal]);
				else
					printf("%s %d %s ---- %s yes ----\n",tree->lexeme, tree->line_num, string_tokens[tree->t], 
						string_tokens[tree->parent->t + count_terminal]);
			}
		}
		else
			printf("---- ---- ---- ---- %s no %s\n", tree->parent == NULL ? "ROOT\0" : string_tokens[tree->parent->t + count_terminal], 
				string_tokens[tree->t + count_terminal]);
		
		int i;
		for(i=1; i<tree->child_count; i++)
			printTreeInorder(tree->child[i]);
	}
}

void setFields(TREENODEPTR tree, struct token **tokens) {
	if (tree != NULL) {

		if (tree->child_count > 0)
			setFields(tree->child[0], tokens);
		
		if(tree->t_or_nt == 1) {
			if (tree->t == e)
				;
			else {
				if ((*tokens)->tokenID == NUM) {
					strcpy(tree->lexeme, (*tokens)->lexeme);
					tree->val.int_val = (*tokens)->val.int_val;
					tree->line_num = (*tokens)->line_num;
				}
				else if ((*tokens)->tokenID == RNUM) {
					strcpy(tree->lexeme, (*tokens)->lexeme);
					tree->val.float_val = (*tokens)->val.float_val;
					tree->line_num = (*tokens)->line_num;
				}
				else {
						while (*tokens && (*tokens)->tokenID == COMMENTMARK)
							*tokens = (*tokens)->next;
					strcpy(tree->lexeme, (*tokens)->lexeme);
					tree->line_num = (*tokens)->line_num;

				}
				
				*tokens = (*tokens)->next;
				while (*tokens && (*tokens)->tokenID == COMMENTMARK)
					*tokens = (*tokens)->next;
			}
		}
		
		int i;
		for(i=1; i<tree->child_count; i++)
			setFields(tree->child[i], tokens);
	}
}

TREENODEPTR generateParseTree(int *ruleList, int *index, int len, TREENODEPTR parent) {
	if (*index == len)
		return NULL;

	TREENODEPTR root = (TREENODEPTR)malloc(sizeof(struct parseTreeNode));
	root->child_count = grammar[ruleList[*index]]->count - 1;
	root->child = (TREENODEPTR*)malloc(root->child_count * sizeof(TREENODEPTR));
	NODEPTR n = grammar[ruleList[*index]]->first;
	root->t = n->t;
	root->t_or_nt = n->t_or_nt;
	root->parent = parent;
	int i=0;
	*index = *index + 1;
	n = n->next;
	while (n) {
		if (n->t_or_nt == 0)
			root->child[i] = generateParseTree(ruleList, index, len, root);
		else {
			root->child[i] = (TREENODEPTR)malloc(sizeof(struct parseTreeNode));
			root->child[i]->child_count = 0;
			root->child[i]->child = NULL;
			root->child[i]->t = n->t;
			root->child[i]->t_or_nt = n->t_or_nt;
			root->child[i]->parent = root;
		}
		i++;
		n = n->next;
	}
	return root;
}
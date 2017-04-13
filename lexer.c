/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lexerDef.h"
// #include "forstring.h"
char *string_tokens_lexer[] = {
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
int flag_temp=0;
int line_num = 1;
struct token *head=NULL, *last=NULL, *current=NULL;

void appendSymbol(int id) {
    struct token *curr = (struct token*)malloc(sizeof(struct token));

    switch(id) {
        case 1 : strcpy(curr->lexeme, ":\0");
            break;
        case 2 : strcpy(curr->lexeme, ":=\0");
            break;
        case 3 : strcpy(curr->lexeme, ",\0");
            break;
        case 4 : strcpy(curr->lexeme, "!=\0");
            break;
        case 5 : strcpy(curr->lexeme, "..\0");
            break;
        case 6 : strcpy(curr->lexeme, ">\0");
            break;
        case 7 : strcpy(curr->lexeme, ">>\0");
            break;
        case 8 : strcpy(curr->lexeme, ">=\0");
            break;
        case 9 : strcpy(curr->lexeme, ";\0");
            break;
        case 10 : strcpy(curr->lexeme, "==\0");
            break;
        case 11 : strcpy(curr->lexeme, "-\0");
            break;
        case 12 : strcpy(curr->lexeme, "<\0");
            break;
        case 13 : strcpy(curr->lexeme, "<<\0");
            break;
        case 14 : strcpy(curr->lexeme, "<=\0");
            break;
        case 15 : strcpy(curr->lexeme, "+\0");
            break;
        case 16 : strcpy(curr->lexeme, "*\0");
            break;
        case 17 : strcpy(curr->lexeme, "**\0");
            break;
        case 18 : strcpy(curr->lexeme, "/\0");
            break;
        case 19 : strcpy(curr->lexeme, "[\0");
            break;
        case 20 : strcpy(curr->lexeme, "]\0");
            break;
        case 21 : strcpy(curr->lexeme, "(\0");
            break;
        case 22 : strcpy(curr->lexeme, ")\0");
            break;
        case 23 : strcpy(curr->lexeme, "<<<\0");
            break;
        case 24 : strcpy(curr->lexeme, ">>>\0");
            break;
    }
    curr->tokenID = NUM+id;
    curr->next = NULL;
    curr->line_num = line_num;
    curr->prev = last;
    last->next = curr;
    last = last->next;
}

void addKeyword(int id, char *keyword) {
    struct token *curr = (struct token*)malloc(sizeof(struct token));

    int i=0;
    strcpy(curr->lexeme, keyword);
    curr->tokenID = id;
    curr->next = NULL;
    curr->line_num = line_num;
    curr->prev = last;
    last->next = curr;
    last = last->next;
}

int isKeyword(char *word) {
    char *list[30] = {"declare\0", "driver\0", "program\0", "for\0", "start\0", "end\0", "module\0", "get_value\0", "print\0",
                    "use\0", "with\0", "parameters\0", "true\0", "false\0", "takes\0", "input\0", "returns\0", "AND\0", "OR\0", 
                    "switch\0", "case\0", "break\0", "default\0", "while\0", "integer\0", "real\0", "boolean\0", "of\0", "array\0",
                    "in\0" };
    int i=0;
    while (i < 30)
        if (strcmp(list[i++], word) == 0)
            return i-1;
    return -1;
}

void addID(char *id) {
    struct token *curr = (struct token*)malloc(sizeof(struct token));

    curr->tokenID = ID;
    strcpy(curr->lexeme, id);
    curr->next = NULL;
    curr->line_num = line_num;
    curr->prev = last;
    last->next = curr;
    last = last->next;
}

void addNUM(int n_or_r, char *num, float value) {
    struct token *curr = (struct token*)malloc(sizeof(struct token));

    if (n_or_r == 1) {
        curr->tokenID = NUM;
        curr->val.int_val = (int)value;
    }
    else {
        curr->tokenID = RNUM;
        curr->val.float_val = value;
    }
    strcpy(curr->lexeme, num);
    curr->next = NULL;
    curr->line_num = line_num;
    curr->prev = last;
    last->next = curr;
    last = last->next;
}

void matchSymbols(char *buf) {
    int index=0;
    int len = strlen(buf);
    char front = buf[index];

    while (index < len) {

        if (flag_temp == 1) {
            while (!((buf[index] == '*') && (buf[index+1] == '*')) && (index < len))
                index++;
            if (index == len)
                index--;
            front = buf[index];
        }

        if ((front >= 'A' && front <= 'Z') || (front >= 'a' && front <= 'z')) {
            char id[20];
            int i=0;
            id[i++] = front;
            index++;
            while ((buf[index]>='A' && buf[index]<='Z') || (buf[index]>='a' && buf[index]<='z') || buf[index]=='_' || (buf[index]>='0' && buf[index]<='9') && buf[index]!='\n')
                id[i++] = buf[index++];
            
            id[i] = '\0';
            int type = isKeyword(id);
            if (type != -1)
                addKeyword(type, id);
            else if (i > 8) {
                printf(" Identifier at line %d is longer than the prescribed length\n", line_num);
            }
            else
                addID(id);
            index--;
        }
        
        else if (front >= '0' && front <= '9') {
            char num[20];
            int i=0;
            int temp1, temp2;
            temp1 = temp2 = 0;
            int dec=0, power=0;
            num[i++] = front;
            temp1 = temp1*10 + front-'0';
            front = buf[++index];
            while (front >= '0' && front <= '9') {
                temp1 = temp1*10 + front-'0';
                num[i++] = front;
                front = buf[++index];
            }

            if (front != '.') {
                num[i] = '\0';
                // printf("%d\n",temp1);
                addNUM(1,num,temp1);
            }

            else {
                if (buf[index+1] == '.') {
                    num[i] = '\0';
                    // printf("%d\n",temp1);
                    addNUM(1,num,temp1);
                }
                else {
                    num[i++] = '.';
                    front = buf[++index];
                    while (front >= '0' && front <= '9') {
                        temp2 = temp2*10 + front-'0';
                        dec++;
                        num[i++] = front;
                        front = buf[++index];
                    }
                    if (front == 'E' || front == 'e') {
                        num[i++] = front;
                        front = buf[++index];
                        int sign=0;
                        if (front == '+' || front == '-') {
                            if (front == '-')
                                sign = 1;
                            num[i++] = front;
                            front = buf[++index];
                        }
                        while (front >= '0' && front <= '9') {
                            power = power*10 + front - '0';
                            num[i++] = front;
                            front = buf[++index];
                        }
                        if (sign)
                            power = -1 * power;
                    }
                    num[i] = '\0';
                    float number = (float)temp1 + (float)(temp2/(pow(10,dec)));
					number *= pow(10,power);
                    // printf("%f\n",number);
                    addNUM(0,num,number);
                }
            }
            index--;
        }
        
        else if (front == ':') {
            if (buf[index+1] == '=') {
                index++;
                appendSymbol(2);
            }
            else
                appendSymbol(1);
        }
        else if (front == ',')
            appendSymbol(3);
        
        else if (front == '!') {
            if (buf[index+1] == '=') {
                index++;
                appendSymbol(4);
            }
            else
                printf("Unknown Symbol !%c at line %d", buf[index+1], line_num);
        }

        else if (front == '.') {
            if (buf[index+1] == '.') {
                index++;
                appendSymbol(5);
            }
            else
                printf("Unknown Symbol .%c at line %d", buf[index+1], line_num);
        }

        else if (front == '>') {
			if (buf[index+1] == '=') {			
			    appendSymbol(8);
				index++;			
			}
			else if (buf[index+1] == '>') {
				if (buf[index+2]=='>') {			
				    appendSymbol(24);
					index+=2;
				}
				else {
				    appendSymbol(7);
					index++;
				}
			}
			else
                appendSymbol(6);
		}
        else if (front == ';')
            appendSymbol(9);
        
        else if (front == '=') {
            if (buf[index+1] == '=') {
                index++;
                appendSymbol(10);
            }
            else
                printf("Unknown Symbol =%c at line %d", buf[index+1], line_num);
        }
        else if (front == '-')
            appendSymbol(11);

        else if (front == '<') {
			if (buf[index+1] == '=') {			
			    appendSymbol(14);
				index++;			
			}
			else if (buf[index+1] == '<') {
				if (buf[index+2]=='<') {			
					appendSymbol(23);
					index+=2;
				}
				else {
				    appendSymbol(13);
					index++;
				}
			}
			else
                appendSymbol(12);
		}

        else if (front == '*') {
            if (buf[index+1] == '*') {
                appendSymbol(17);
                index++;
                if (!flag_temp)
                    flag_temp = 1;
                else {
                    flag_temp = 0;
                    front = buf[++index];
                    continue;
                }

                while (!((buf[index] == '*') && (buf[index+1] == '*')) && (index < len))
				    index++;
                index--;
            }
            else
                appendSymbol(16);
        }

        else if (front == '+')
            appendSymbol(15);	
        else if (front == '/')
            appendSymbol(18);
        else if (front == '[')
            appendSymbol(19);
        else if (front == ']')
            appendSymbol(20);
        else if (front == '(')
            appendSymbol(21);
        else if (front == ')')
            appendSymbol(22);
        else if (front == '\n')
            line_num++;
        // else if (front != ' ' || front != '\t')
        //     printf("Unknown pattern %c at line %d\n", front, line_num);
        
        front = buf[++index];
		while (front == ' ' || front == '\t')
            front = buf[++index];
    }
}

FILE* getStream(FILE *fp, char *buf) {
    buf[0] = '\0';

    fgets(buf, BUFFER_SIZE+1, fp);
    if (buf[0] == '\0')
        return NULL;

    if (strlen(buf) == BUFFER_SIZE) {
        int i = BUFFER_SIZE-1;
        while (!(buf[i] != ' ' || buf[i] != '\t' || buf[i] != '\n'))
            i--;

        buf[i+1] = '\0';
        fseek(fp, -(BUFFER_SIZE-1-i), SEEK_CUR);
    }
    return fp;
}

void generateList(char *file) {
    FILE *read = fopen(file,"r");
    char buf[BUFFER_SIZE+1];
    head = (struct token*)malloc(sizeof(struct token));
    last = current = head;
    read = getStream(read, buf);
    while(read != NULL) {
        matchSymbols(buf);
        read = getStream(read, buf);
    }
}

struct token* getNextToken() {
    if (current) {
        current = current->next;
        return current;
    }
    else {
        current = head;
        return current;
    }
}

void printlist() {
    struct token *temp = head->next;
    // FILE *fp = fopen("TokenList.txt", "w");
    while (temp != NULL) {
        printf("%s %s %d\n",string_tokens_lexer[temp->tokenID], temp->lexeme, temp->line_num);
        // fprintf(fp, "%s %s %d\n",string_tokens_lexer[temp->tokenID], temp->lexeme, temp->line_num);
        temp = temp->next;
    }
    // fclose(fp);
}

void removeComments(char *testcaseFile, char *cleanFile) {
    FILE *read = fopen(testcaseFile, "r");
    FILE *out = fopen(cleanFile, "w");

    char buf[BUFFER_SIZE+1];
    read = getStream(read, buf);
    while (read) {
        int index=0;
        int flag = 0;
        int len = strlen(buf);
        while (index < len) {
            if (flag == 1) {
                while ((buf[index] != '*' || buf[index] != '*') && index < len)
                    index++;
            }
            while ((buf[index] != '*' || buf[index] != '*') && index < len)
                fprintf(out, "%c", buf[index++]);
            if (index >= len)
                break;
            else {
                if (flag)
                    flag = 0;
                else
                    flag = 1;
                index+=2;
            }
        }
        read = getStream(read, buf);
    }
}

void removeCommentsConsole(char *testcaseFile) {
    FILE *read = fopen(testcaseFile, "r");
    // FILE *out = fopen(cleanFile, "w");
    int flag = 0;
    char buf[BUFFER_SIZE+1];
    read = getStream(read, buf);
    while (read) {
        int index=0;
        // int flag = 0;
        int len = strlen(buf);
        while (index < len) {
            if (flag == 1) {
                while ((buf[index] != '*' || buf[index+1] != '*') && index < len)
                    index++;
            }
            while ((buf[index] != '*' || buf[index+1] != '*') && index < len)
                printf("%c", buf[index++]);
            if (index >= len)
                break;
            else {
                if (flag)
                    flag = 0;
                else
                    flag = 1;
                index+=2;
            }
        }
        read = getStream(read, buf);
    }
}

struct token* getFirst() {
    return head;
}
struct token *returnHead(){
    return head;
}
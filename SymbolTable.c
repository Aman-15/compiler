/*
1. Data Type for arrays...done.
2. error handling...Done at the symbol table level.
3. checking id in function parameter...done.
4. CHeck if used module is ever defined...during ast.
5. Iterative and switch statements, iterator can't be declared.
6. Testcase4_new - Switch-Case "index" not found.
*/

/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include "SymbolTable.h"

char *string_tokens1[] = {
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
int scopeId = 0;

/********* Stack Implementation ************/

TREENODEPTR top_st(struct tnll *stack) {       //first element of the linked list
	if(stack->first == NULL)
		return NULL;

	return stack->first->node;
}

void push_st(struct tnll *stack, TREENODEPTR n) {
	struct tnllnode *newone = (struct tnllnode *)malloc(sizeof(struct tnllnode));
	newone->node = n;
	struct tnllnode *temp = stack->first;
	stack->first = newone;
	newone->next = temp;
	stack->count++;
}

void pop_st(struct tnll *stack) {
	if(stack->first == NULL) {
		printf("STACK IS EMPTY!! EXITING\n");
		return;
	}
	struct tnllnode *temp = stack->first;
	stack->first = stack->first->next;
	temp->next = NULL;
	free(temp);
	stack->count--;
}

int isEmpty(struct tnll *stack) {
    return stack->count == 0 ? 1 : 0;
}
//End of Stack

struct FuncTable* findInFunctionList(char *name) {
    struct FuncTable *f = funcList;
    while (f) {
        if (strcmp(f->name, name) == 0)
            return f;
        f = f->next;
    }

    return NULL;
}

//Recursively checks for the id upto the global tabel.
int checkIdentifierifDeclared(struct IdTable *table, char *name) {
    if(table == NULL)
        return 0;

    if (checkinTable(table, name) == 0) {
        if (table->parent)
            return checkinTable(table->parent, name);
        return 0;
    }
    return 1;
}

//Checks for the id in the current table only.
int checkinTable(struct IdTable *table, char *name) {
    if (table == NULL)
        return 0;

    struct IdTuple *tuple = table->idTuple;

    while (tuple) {
        if (strcmp(tuple->name, name) == 0)
            return 1;
        tuple = tuple->next;
    }

    return 0;
}

int checkInParameters(struct FuncTable *curr, char *name) {
    struct Parameters *out = curr->out;
    struct Parameters *in = curr->in;

    while (out) {
        if (strcmp(out->name, name) == 0)
            return 1;
        out = out->next;
    }

    while (in) {
        if (strcmp(in->name, name) == 0)
            return 1;
        in = in->next;
    }

    return 0;
}

//Adds a table to the funcList
struct FuncTable* addFunc(struct FuncTable* currFuncTable) {
    if (funcList == NULL) {
        currFuncTable = (struct FuncTable*)malloc(sizeof(struct FuncTable));
        funcList = currFuncTable;
    }
    else {
        while (currFuncTable->next)
            currFuncTable = currFuncTable->next;
        currFuncTable->next = (struct FuncTable*)malloc(sizeof(struct FuncTable));
        currFuncTable = currFuncTable->next;
    }

    strcpy(currFuncTable->name, "\0");
    currFuncTable->out = NULL;
    currFuncTable->in = NULL;
    currFuncTable->status = none;
    currFuncTable->idTable = NULL;
    currFuncTable->next = NULL;
    return currFuncTable;
}

//Sets the parameters (both in and out) for the given function
void addInParameter(struct FuncTable *currFuncTable, struct Parameters *inp, struct Parameters *outp){
    currFuncTable->out = outp;
    currFuncTable->in = inp;
}

//Returns a new Idtable* with the parent as the currIdTable
struct IdTable* getNewTable() {
    struct IdTable *temp = (struct IdTable*)malloc(sizeof(struct IdTable));
    temp->scopeId = ++scopeId;
    temp->count = 0;
    temp->child = NULL;
    temp->next = NULL;
    temp->parent = NULL;
    temp->idTuple = NULL;
    return temp;
}

//Adds a table as a child to represent a nested scope
struct IdTable* addNestedScope(struct IdTable *currIdTable) {
    //Assuming Driver table (aka global) is already made
    
    struct IdTable *temp = getNewTable();
    temp->parent = currIdTable;

    if (currIdTable->child) {               //It has some children
        struct IdTable *temp1 = currIdTable->child;
        while (temp1->next)
            temp1 = temp1->next;
        temp1->next = temp;
    }
    else
        currIdTable->child = temp;

    currIdTable->count++;

    return temp;
}

//Passing the current table will just add a tuple to the table
void addTupleEntry(struct IdTable *currIdTable, char *name, int line_dec, int line_ref, enum enum_token type, int arr_or_not, int start, int end) {
    struct IdTuple *temp = (struct IdTuple*)malloc(sizeof(struct IdTuple));
    temp->line_dec = line_dec;
    temp->line_ref = line_ref;
    temp->type = type;
    strcpy(temp->name, name);
    temp->next = currIdTable->idTuple;
    currIdTable->idTuple = temp;
    temp->start = start;
    temp->end = end;
    temp->arr_or_not = arr_or_not;
}

//pop_stulating the symbol tables
void makeTable(TREENODEPTR treeNode) {
    
    struct tnll *stack = (struct tnll*)malloc(sizeof(struct tnll));
    stack->count = 0;
    stack->first = NULL;
    push_st(stack, treeNode);
    struct IdTable *currIdTable = NULL;
    struct FuncTable *currFuncTable = NULL;
    global = NULL;
    funcList = NULL;

    while (!isEmpty(stack)) {


        TREENODEPTR temp = top_st(stack);
        
        if (temp->t_or_nt == 0) {   //If it is a non-terminal, then push_st all the children
            // printf("%d %s\n", stack->count, string_tokens1[top_st(stack)->t + count_terminal]);
            pop_st(stack);
            
            int i = temp->child_count;
            while (--i >= 0) {
                push_st(stack, temp->child[i]);
            }
        }

        else {                      //If it is a terminal, then pop_st and just process it
            // printf("%d %s\n", stack->count, string_tokens1[top_st(stack)->t]);
            pop_st(stack);

            //Starting with DECLARE
            if (temp->t == DECLARE) {
                
                if (top_st(stack)->t == MODULE) { //Eg.1 declare module compute;
                    pop_st(stack);     //pop_st module so top_st is ID
                    TREENODEPTR n = top_st(stack);
                    pop_st(stack); //function id pop_stped

                    if (findInFunctionList(n->lexeme) == NULL) {
                        //Can be inserted
                        currFuncTable = addFunc(currFuncTable);
                        strcpy(currFuncTable->name, n->lexeme);
                        currFuncTable->status = declared;
                    }
                    else {
                        //ERROR (Re declared)
                        printf("1 Re-declaration Error\n");
                    }
                }

                else {                          //Eg.2 declare var_1_, var_2_, var_3_:integer;

                    //handle if id declared is same as function parameter
                    TREENODEPTR _dtype = top_st(stack)->parent->child[3];
                    TREENODEPTR _idlist = top_st(stack)->parent->child[1]; //id list is the top_st of stack also

                    pop_st(stack); //IdList pop_stped
                    pop_st(stack); //COLON pop_stped
                    pop_st(stack); //DataType pop_stped
                    pop_st(stack); //SEMICOLON pop_stped

                    if (checkinTable(currIdTable, _idlist->child[0]->lexeme) == 1) {
                        //error: already declared
                        printf("2 Already Declared %s\n", _idlist->child[0]->lexeme);
                    }
                    else if (currIdTable->parent == NULL && checkInParameters(currFuncTable, _idlist->child[0]->lexeme) == 1) {
                        //error: already declared
                        printf("2.1 Already Declared in Parameters %s\n", _idlist->child[0]->lexeme);

                    }
                    else {
                        //Handle Arrays separately
                        if (_dtype->child[0]->t == ARRAY) {
                            int start = _dtype->child[2]->child[0]->val.int_val;
                            int end = _dtype->child[2]->child[2]->val.int_val;
                            addTupleEntry(currIdTable, _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[5]->child[0]->t, 1, start, end);
                        }
                        else
                            addTupleEntry(currIdTable, _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[0]->t, 0, -1, -1);
                    }
                    TREENODEPTR cur = _idlist->child[1];
                    while(cur->child[0]->t != e) {

                        TREENODEPTR n = cur->child[1];
                        if (checkinTable(currIdTable, n->lexeme) == 1) {
                            //Error (Already declared)
                        printf("3 Already Declared %s\n", n->lexeme);
                        }
                        else if (currIdTable->parent == NULL && checkInParameters(currFuncTable, _idlist->child[0]->lexeme) == 1) {
                            //error: already declared
                            printf("3.1 Already Declared in Parameters %s\n", _idlist->child[0]->lexeme);

                        }
                        else {
                            //Handle Arrays separately
                            if (_dtype->child[0]->t == ARRAY) {
                                int start = _dtype->child[2]->child[0]->val.int_val;
                                int end = _dtype->child[2]->child[2]->val.int_val;
                                addTupleEntry(currIdTable, _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[5]->child[0]->t, 1, start, end);
                            }
                            else
                                addTupleEntry(currIdTable, _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[0]->t, 0, -1, -1);
                        }
                        cur = cur->child[2];
                    }
                }
            }

            else if (temp->t == START) {
                //Now we have a new Scope

                if (currFuncTable->idTable == NULL) {//The current function has no scope yet
                    currFuncTable->idTable = getNewTable();
                    currIdTable = currFuncTable->idTable; //change the scope
                }

                else {
                    currIdTable = addNestedScope(currIdTable);
                }
            }

            else if (temp->t == END) {
                currIdTable = currIdTable->parent;
            }

            else if (temp->t == ID) {
                
                if (temp->parent->t == input_plist) {
                    TREENODEPTR _dtype = top_st(stack)->parent->child[2];
                    pop_st(stack); //colon pop_stped
                    pop_st(stack); //dataType pop_stped
                    TREENODEPTR cur = top_st(stack);
                    pop_st(stack); //ipList2 pop_stped

                    struct Parameters *in_start = (struct Parameters *)malloc(sizeof(struct Parameters));
                    struct Parameters *in = in_start;
                    strcpy(in->name, temp->lexeme);
                    if (_dtype->child[0]->t == ARRAY) {
                        in->start = _dtype->child[2]->child[0]->val.int_val;
                        in->end = _dtype->child[2]->child[2]->val.int_val;
                        in->type = _dtype->child[5]->child[0]->t;
                    }
                    else {
                        in->type = _dtype->child[0]->t;
                    }

                    while(cur->child[0]->t != e){
                        TREENODEPTR n = cur->child[1];
                        _dtype = cur->child[3];
                        if (checkinTable(currIdTable, n->lexeme) == 1) {
                            //Error (Already declared)
                        printf("4 Already Declared %s\n", n->lexeme);
                        }
                        else {
                            //Handle Arrays separately
                            in->next = (struct Parameters *)malloc(sizeof(struct Parameters));
                            strcpy(in->next->name, n->lexeme);
                            if (_dtype->child[0]->t == ARRAY) {
                                in->next->start = _dtype->child[2]->child[0]->val.int_val;
                                in->next->end = _dtype->child[2]->child[2]->val.int_val;
                                in->next->type = _dtype->child[5]->child[0]->t;
                            }
                            else {
                                in->next->type = _dtype->child[0]->t;
                            }
                            in = in->next;
                        }
                        cur = cur->child[4];
                    }
                    currFuncTable->in = in_start;
                }

                else if (temp->parent->t == output_plist) {
                    TREENODEPTR _dtype = top_st(stack)->parent->child[2];
                    pop_st(stack); //colon pop_stped
                    pop_st(stack); //dataType pop_stped
                    TREENODEPTR cur = top_st(stack);
                    pop_st(stack); //ipList2 pop_stped

                    struct Parameters *out_start = (struct Parameters *)malloc(sizeof(struct Parameters));
                    struct Parameters *out = out_start;
                    strcpy(out->name, temp->lexeme);
                    if (_dtype->child[0]->t == ARRAY) {
                        out->start = _dtype->child[2]->child[0]->val.int_val;
                        out->end = _dtype->child[2]->child[2]->val.int_val;
                        out->type = _dtype->child[5]->child[0]->t;
                    }
                    else {
                        out->type = _dtype->child[0]->t;
                    }

                    while(cur->child[0]->t != e){
                        TREENODEPTR n = cur->child[1];
                        _dtype = cur->child[3];
                        if (checkinTable(currIdTable, n->lexeme) == 1) {
                            //Error (Already declared)
                        printf("5 Already Declared %s\n", n->lexeme);
                        }
                        else {
                            //Handle Arrays separately
                            
                            out->next = (struct Parameters *)malloc(sizeof(struct Parameters));
                            strcpy(out->next->name, n->lexeme);
                            if (_dtype->child[0]->t == ARRAY) {
                                out->next->start = _dtype->child[2]->child[0]->val.int_val;
                                out->next->end = _dtype->child[2]->child[2]->val.int_val;
                                out->next->type = _dtype->child[5]->child[0]->t;
                            }
                            else {
                                out->next->type = _dtype->child[0]->t;
                            }

                            out = out->next;
                        }
                        cur = cur->child[4];
                    }
                    currFuncTable->out = out_start;
                }

                else {
                    if (checkIdentifierifDeclared(currIdTable, temp->lexeme) == 0 && checkInParameters(currFuncTable, temp->lexeme) == 0) {
                        //Error
                        printf("6 Variable Not Declared %s %s at line %d\n", string_tokens1[temp->t], temp->lexeme, temp->line_num);
                    }
                }
            }
            else if(temp->t == USE) {
                pop_st(stack); //module pop_stped
                TREENODEPTR n = top_st(stack); //ID of function
                pop_st(stack); //function id pop_stped

                if(findInFunctionList(n->lexeme) == NULL){
                    //error
                    printf("7 No declaration of the function %s\n", n->lexeme);
                }
                //Handle if module is never defined
            }
            else if(temp->t == DEF){
                pop_st(stack); //module pop_stped
                TREENODEPTR n = top_st(stack); //function id stored
                pop_st(stack); //function id pop_stped

                struct FuncTable *f = findInFunctionList(n->lexeme);
                if(f){
                    if(f->status == defined){
                        //error: redifining a function
                        printf("8 Re-defined %s\n", n->lexeme);
                    }
                    else{
                        currFuncTable = f;
                        currFuncTable->status = defined;

                    }
                }
                else {
                    //add to function list and give memory
                    currFuncTable = addFunc(currFuncTable);
                    strcpy(currFuncTable->name, n->lexeme);
                    currFuncTable->status = defined;

                }
            }

            else if(temp->t == DRIVERDEF) {
                currFuncTable = addFunc(currFuncTable);
                strcpy(currFuncTable->name, "driver");
                currFuncTable->status = defined;
            }
        }
    }
}
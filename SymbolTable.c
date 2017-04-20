/*
1. Data Type for arrays...done.
2. error handling...Done at the symbol table level.
3. checking id in function parameter...done.
4. CHeck if used module is ever defined...during ast...not done.
5. Iterative and switch statements, iterator can't be declared....not done.
6. Testcase4_new - Switch-Case "index" not found....not done.
7. Redifinition of index int interative statements not done.
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

int offset;

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

int getWidthTuple(struct IdTuple *tuple) {
    int width;
    switch (tuple->type) {
        case BOOLEAN : width = 1;
            break;
        case REAL : width = 4;
            break;
        case INTEGER : width = 2;
            break;
        default:
            break;
    }
    if (tuple->arr_or_not == 0)
        return width;
    else
        return tuple->end * width;
}

int getWidthParameters(struct Parameters *param) {
    int width;
    switch (param->type) {
        case BOOLEAN : width = 1;
            break;
        case REAL : width = 4;
            break;
        case INTEGER : width = 2;
            break;
        default:
            break;
    }
    if (param->arr_or_not == 0)
        return width;
    else
        return param->end * width;
}

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
int checkIdentifierifDeclared(struct IdTable *table, TREENODEPTR currnode, char *name) {
    if(table == NULL){
        // if(strcmp(name,"var_2_") == 0){
        //     printf("because of NULL\n");
        // }
        return 0;
    }

    if (checkinTable(table, name) == 0) {
        if (table->parent){
            // if(strcmp(name,"var_2_") == 0){
            //     printf("because parent\n");
            // }
            return checkIdentifierifDeclared(table->parent, currnode, name);
        }
        // if(strcmp(name,"var_2_") == 0){
        //         printf("because parent is null and not in current\n");
        // }
        return 0;
    }
    struct IdTuple *tuple = table->idTuple;
    while (tuple) {
        if (strcmp(tuple->name, name) == 0){
            currnode->tuple = tuple;
            break;
        }
        tuple = tuple->next;
    }
    return 1;
}

//Checks for the id in the current table only.
int checkinTable(struct IdTable *table, char *name) {
    if (table == NULL)
        return 0;

    // printf("%d %s\n", table->scopeId, name);
    struct IdTuple *tuple = table->idTuple;
    while (tuple) {
        if (strcmp(tuple->name, name) == 0)
            return 1;
        tuple = tuple->next;
    }

    return 0;
}

struct Parameters* checkInParameters(struct FuncTable *curr, char *name) {
    struct Parameters *out = curr->out;
    struct Parameters *in = curr->in;

    while (out) {
        if (strcmp(out->name, name) == 0)
            return out;
        out = out->next;
    }

    while (in) {
        if (strcmp(in->name, name) == 0)
            return in;
        in = in->next;
    }

    return NULL;
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
    temp->count = 0;
    temp->child = NULL;
    temp->next = NULL;
    temp->parent = NULL;
    temp->idTuple = NULL;
    temp->idTupleLast = NULL;
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
void addTupleEntry(struct IdTable *currIdTable, TREENODEPTR currnode, char *name, int line_dec, int line_ref, enum enum_token type, int arr_or_not, int start, int end) {
    struct IdTuple *temp = (struct IdTuple*)malloc(sizeof(struct IdTuple));
    temp->line_dec = line_dec;
    temp->line_ref = line_ref;
    temp->type = type;
    strcpy(temp->name, name);
    temp->start = start;
    temp->end = end;
    temp->arr_or_not = arr_or_not;
    temp->offset = offset;
    temp->width = getWidthTuple(temp);
    offset += getWidthTuple(temp);
    temp->next = NULL;

    if (currIdTable->idTuple == NULL) {
        currIdTable->idTuple = currIdTable->idTupleLast = temp;
    }
    else {
        currIdTable->idTupleLast->next = temp;
        currIdTable->idTupleLast = temp;
    }
    
    if (currnode)
        currnode->tuple = temp;
}

void checkIfOutAssigned() {
    struct FuncTable *curr = funcList;
    while (curr != NULL) {
        if (strcmp(curr->name, "driver") == 0)
            curr = curr->next;

        else {
            struct Parameters *out = curr->out;
            while (out) {
                if (out->assigned == 0) {
                    printf("line:%d Output Parameter \"%s\" is never assigned a value\n", out->line_num, out->name);
                }
                out = out->next;
            }
            curr = curr->next;
        }
    }
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
            pop_st(stack);
            
            int i = temp->child_count;
            while (--i >= 0) {
                push_st(stack, temp->child[i]);
            }
        }

        else {                      //If it is a terminal, then pop_st and just process it
            pop_st(stack);

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
                        printf("line:%d \"%s\" Re-declaration Error\n", n->line_num, n->lexeme);
                    }
                }

                else {                          //Eg.2 declare var_1_, var_2_, var_3_:integer;
                    TREENODEPTR _dtype = top_st(stack)->parent->child[3];
                    TREENODEPTR _idlist = top_st(stack); //id list is the top_st of stack also

                    pop_st(stack); //IdList pop_stped
                    pop_st(stack); //COLON pop_stped
                    pop_st(stack); //DataType pop_stped
                    pop_st(stack); //SEMICOLON pop_stped

                    if (checkIdentifierifDeclared(currIdTable, _idlist->child[0], _idlist->child[0]->lexeme) == 1) {
                        //error: already declared
                        printf("line:%d \"%s\" Re-declaration Error\n", _idlist->child[0]->line_num, _idlist->child[0]->lexeme);
                    }
                    else if (checkInParameters(currFuncTable, _idlist->child[0]->lexeme) != NULL) {
                        //error: already declared
                        printf("line:%d \"%s\" Re-declaration of Parameters Error\n", _idlist->child[0]->line_num, _idlist->child[0]->lexeme);

                    }
                    else {
                        //Handle Arrays separately
                        if (_dtype->child[0]->t == ARRAY) {
                            int start = _dtype->child[2]->child[0]->val.int_val;
                            int end = _dtype->child[2]->child[2]->val.int_val;
                            addTupleEntry(currIdTable, _idlist->child[0], _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[5]->child[0]->t, 1, start, end);
                        }
                        else
                            addTupleEntry(currIdTable, _idlist->child[0], _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[0]->t, 0, -1, -1);
                    }
                    TREENODEPTR cur = _idlist->child[1];
                    while(cur->child[0]->t != e) {

                        TREENODEPTR n = cur->child[1];

                        if (checkIdentifierifDeclared(currIdTable,n, n->lexeme) == 1) {
                            printf("line:%d \"%s\" Re-declaration Error\n", n->line_num, n->lexeme);
                        }
                        else if (currIdTable->parent == NULL && checkInParameters(currFuncTable, n->lexeme) != NULL) {
                            printf("line:%d \"%s\" Re-declaration of Parameters Error\n", n->line_num, n->lexeme);
                        }
                        else {
                            if (_dtype->child[0]->t == ARRAY) {
                                int start = _dtype->child[2]->child[0]->val.int_val;
                                int end = _dtype->child[2]->child[2]->val.int_val;
                                addTupleEntry(currIdTable, n, n->lexeme, n->line_num, 0, _dtype->child[5]->child[0]->t, 1, start, end);
                      
                            }
                            else
                                addTupleEntry(currIdTable, n, n->lexeme, n->line_num, 0, _dtype->child[0]->t, 0, -1, -1);
                        }
                        cur = cur->child[2];
                    }
                }
            }

            else if (temp->t == START) {

                if (currFuncTable->idTable == NULL) {//The current function has no scope yet
                    currFuncTable->idTable = getNewTable();
                    currIdTable = currFuncTable->idTable; //change the scope
                    currIdTable->nesting = 1;
                    currIdTable->beg = temp->line_num;
                }

                else {
                    currIdTable = addNestedScope(currIdTable);
                    currIdTable->beg = temp->line_num;
                    currIdTable->nesting = currIdTable->parent->nesting+1;
                }
            }

            else if (temp->t == END) {
                currIdTable->last = temp->line_num;
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
                        in->arr_or_not = 1;
                    }
                    else {
                        in->type = _dtype->child[0]->t;
                        in->arr_or_not = 0;
                    }
                    in->offset = offset;
                    in->width = getWidthParameters(in);
                    offset += getWidthParameters(in);
                    in->line_num = temp->line_num;
                    in->assigned = 0;
                    currFuncTable->in = in_start;

                    while(cur->child[0]->t != e){
                        TREENODEPTR n = cur->child[1];
                        _dtype = cur->child[3];
                        if (checkInParameters(currFuncTable, n->lexeme) != NULL) {
                            printf("line:%d \"%s\" Already in the input parameter list\n", n->line_num, n->lexeme);
                        }
                        else {
                            in->next = (struct Parameters *)malloc(sizeof(struct Parameters));
                            strcpy(in->next->name, n->lexeme);
                            if (_dtype->child[0]->t == ARRAY) {
                                printf("%s\n", in->next->name);
                                in->next->start = _dtype->child[2]->child[0]->val.int_val;
                                in->next->end = _dtype->child[2]->child[2]->val.int_val;
                                in->next->arr_or_not = 1;
                                in->next->type = _dtype->child[5]->child[0]->t;
                            }
                            else {
                                in->next->type = _dtype->child[0]->t;
                                in->next->arr_or_not = 0;
                            }
                            in->next->line_num = n->line_num;
                            in->next->offset = offset;
                            in->next->width = getWidthParameters(in->next);
                            in->next->assigned = 0;
                            offset += getWidthParameters(in->next);
                            in = in->next;
                        }
                        cur = cur->child[4];
                        currFuncTable->in = in_start;
                    }
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
                        out->arr_or_not = 1;
                    }
                    else {
                        out->type = _dtype->child[0]->t;
                        out->arr_or_not = 0;
                    }
                    out->line_num = temp->line_num;
                    out->offset = offset;
                    out->width = getWidthParameters(out);
                    offset += getWidthParameters(out);
                    currFuncTable->out = out_start;
                    out->assigned = 0;

                    while(cur->child[0]->t != e){
                        TREENODEPTR n = cur->child[1];
                        _dtype = cur->child[3];
                        if (checkInParameters(currFuncTable, n->lexeme) != NULL) {
                            printf("line:%d \"%s\" Already in the parameter list\n", n->line_num, n->lexeme);
                        }
                        else {
                            
                            out->next = (struct Parameters *)malloc(sizeof(struct Parameters));
                            strcpy(out->next->name, n->lexeme);
                            if (_dtype->child[0]->t == ARRAY) {
                                out->next->start = _dtype->child[2]->child[0]->val.int_val;
                                out->next->end = _dtype->child[2]->child[2]->val.int_val;
                                out->next->type = _dtype->child[5]->child[0]->t;
                                out->next->arr_or_not = 1;
                            }
                            else {
                                out->next->type = _dtype->child[0]->t;
                                out->next->arr_or_not = 0;
                            }
                            out->next->line_num = n->line_num;
                            out->next->offset = offset;
                            out->next->width = getWidthParameters(out->next);
                            out->next->assigned = 0;
                            offset += getWidthParameters(out->next);
                            out = out->next;
                        }
                        cur = cur->child[4];
                        currFuncTable->out = out_start;
                    }
                }

                else {
                    if (checkIdentifierifDeclared(currIdTable, temp, temp->lexeme) == 0 && checkInParameters(currFuncTable, temp->lexeme) == NULL) {
                        printf("line:%d variable \"%s\" not Declared\n", temp->line_num, temp->lexeme);
                    }
                    else if (temp->parent->t == assignmentStmt) {
                        struct Parameters *temp1 = checkInParameters(currFuncTable, temp->lexeme);
                        if (temp1 != NULL)
                            temp1->assigned = 1;
                    }
                }

            }
            else if(temp->t == USE) {
                pop_st(stack); //module pop_stped
                TREENODEPTR n = top_st(stack); //ID of function
                pop_st(stack); //function id pop_stped

                if(findInFunctionList(n->lexeme) == NULL){
                    printf("line:%d module \"%s\" not declared\n", n->line_num, n->lexeme);
                }

                if(strcmp(currFuncTable->name, n->lexeme) == 0){
                    printf("Line: %d Error!! Recursion is not allowed\n", n->line_num);
                }
                //Handle if module is never defined.. to be done after 
            }

            else if(temp->t == DEF){
                pop_st(stack); //module pop_stped
                TREENODEPTR n = top_st(stack); //function id stored
                pop_st(stack); //function id pop_stped

                struct FuncTable *f = findInFunctionList(n->lexeme);
                offset = 0;
                if(f){
                    if(f->status == defined){
                        printf("line:%d module \"%s\" already defined\n", n->line_num, n->lexeme);
                    }
                    else{
                        currFuncTable = f;
                        currFuncTable->status = defined;
                    }
                }
                else {
                    currFuncTable = addFunc(currFuncTable);
                    strcpy(currFuncTable->name, n->lexeme);
                    currFuncTable->status = defined;

                }
            }

            else if(temp->t == DRIVERDEF) {
                currFuncTable = addFunc(currFuncTable);
                offset = 0;
                strcpy(currFuncTable->name, "driver");
                currFuncTable->status = defined;
            }
        }
    }
}

struct FuncTable* getFuncList() {
    return funcList;
}

void printRowParam(struct FuncTable *currFunc, struct Parameters *param, int *sno) {
    if (param->arr_or_not) {
        printf("%2d  %8s  array(%d,%s)  %10s  %d to %d  1  %d  %d\n", ++(*sno), param->name, param->end, string_tokens1[param->type], currFunc->name,
            currFunc->idTable->beg, currFunc->idTable->last, param->width, param->offset);
    }
    else {
        printf("%2d  %8s  %s  %10s  %d to %d  1  %d  %d\n", ++(*sno), param->name, string_tokens1[param->type], currFunc->name,
            currFunc->idTable->beg, currFunc->idTable->last, param->width, param->offset);
    }
}

void printRowTuple(struct FuncTable *currFunc, struct IdTable *currTable, struct IdTuple *tuple, int *sno) {
    if (tuple->arr_or_not) {
        printf("%2d  %8s  array(%d,%s)  %10s  %d to %d  %d  %d  %d\n", ++(*sno), tuple->name, tuple->end, string_tokens1[tuple->type], currFunc->name,
            currTable->beg, currTable->last, currTable->nesting, tuple->width, tuple->offset);
    }
    else {
        printf("%2d  %8s  %s  %10s  %d to %d  %d  %d  %d\n", ++(*sno), tuple->name, string_tokens1[tuple->type], currFunc->name,
            currTable->beg, currTable->last, currTable->nesting, tuple->width, tuple->offset);
    }
}

void printIdTable(struct FuncTable *currFunc, struct IdTable *currTable, int *sno) {
    if (currTable) {
        struct IdTuple *currTuple = currTable->idTuple;
        
        while (currTuple) {
            printRowTuple(currFunc, currTable, currTuple, sno);
            currTuple = currTuple->next;
        }
        printIdTable(currFunc, currTable->child, sno);
        printIdTable(currFunc, currTable->next, sno);
    }
}

void printSymbolTable() {
    int sno = 0;
    struct FuncTable *currFunc = funcList;

    while (currFunc) {
        struct Parameters *in = currFunc->in;
        struct Parameters *out = currFunc->out;
        while (in) {
            printRowParam(currFunc, in, &sno);
            in = in->next;
        }

        while (out) {
            printRowParam(currFunc, out, &sno);
            out = out->next;
        }

        printIdTable(currFunc, currFunc->idTable, &sno);
        currFunc = currFunc->next;
    }
}
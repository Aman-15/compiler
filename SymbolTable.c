/*
1. Data Type for arrays
2. error handling
3. checking id in function parameter
4. CHeck if used module is ever defined
5. Iterative and switch statements, iterator can't be declared
*/

/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include "SymbolTable.h"

int scopeId = 0;

/********* Stack Implementation ************/

TREENODEPTR top(struct tnll *stack) {       //first element of the linked list
	if(stack->first == NULL)
		return NULL;

	return stack->first->node;
}

void push(struct tnll *stack, TREENODEPTR n) {
	struct tnllnode *newone = (struct tnllnode *)malloc(sizeof(struct tnllnode));
	newone->node = n;
	struct tnllnode *temp = stack->first;
	stack->first = newone;
	newone->next = temp;
	stack->count++;
}

void pop(struct tnll *stack) {
	if(stack->first == NULL) {
		printf("STACK IS EMPTY!! EXITING\n");
		return;
	}
	struct tnllnode *temp = stack->first;
	stack->first = stack->first->next;
	temp->next = NULL;
	//free(temp);
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

int checkParameters(struct FuncTable *func, struct Parameters *out, struct Parameters *in) {
    struct Parameters *outList = func->out;
    struct Parameters *inList = func->in;

    //Till both are null
    while (outList && out) {
        
        //If name is a mismatch or type is a mismatch
        if (strcmp(outList->name, out->name) || outList->type != out->type)
            return 0;

        outList = outList->next;
        out = out->next;
    }

    //Still if either is not NULL, then it is a mismatch
    if (outList || out)
        return 0;
    
    //Till both are null
    while (inList && in) {

        //If name is a mismatch or type is a mismatch
        if (strcmp(inList->name, in->name) || inList->type != in->type)
            return 0;
        
        inList = inList->next;
        in = in->next;
    }

    //Still if either is not NULL, then it is a mismatch
    if (inList || in)
        return 0;
    
    //Return true
    return 1;
}

//Recursively checks for the id upto the global tabel.
int checkIdentifierifDeclared(struct IdTable *table, char *name) {
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

//Adds a table to the funcList
struct FuncTable* addFunc(struct FuncTable* currFuncTable) {
    if (funcList == NULL) {
        currFuncTable = (struct FuncTable*)malloc(struct FuncTable);
        funcList = currFuncTable;
    }
    else {
        while (currFuncTable->next)
            currFuncTable = currFuncTable->next;
        currFuncTable->next = (struct FuncTable*)malloc(struct FuncTable);
        currFuncTable = currFuncTable->next;
    }

    currFuncTable->name = NULL;
    currFuncTable->out = NULL;
    currFuncTable->in = NULL;
    currFuncTable->status = NULL;
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
void addTupleEntry(struct IdTable *currIdTable, char *name, int line_dec, int line_ref, enum enum_token type) {
    struct IdTuple *temp = (struct IdTuple*)malloc(sizeof(struct IdTuple));
    temp->line_dec = line_dec;
    temp->line_ref = line_ref;
    temp->type = type;
    strcpy(temp->name, name);
    temp->next = currIdTable->idTuple;
    currIdTable->idTuple = temp;
}

//Populating the symbol tables
void makeTable(TREENODEPTR treeNode) {
    
    struct tnll *stack = (struct tnll*)malloc(sizeof(struct tnll));
    stack->count = 0;
    stack->first = NULL;
    push(stack, treeNode);
    struct IdTable *currIdTable = NULL;
    struct FuncTable *currFuncTable = NULL;
    global = NULL;
    funcList = NULL;

    while (!isEmpty(stack)) {
        
        TREENODEPTR temp = top(stack);
        
        if (temp->t_or_nt == 0) {   //If it is a non-terminal, then push all the children
            pop(stack);
            
            int i = temp->child_count;
            while (--i) {
                push(stack, temp->child[i]);
            }
        }

        else {                      //If it is a terminal, then pop and just process it
            pop(stack);

            //Starting with DECLARE
            if (temp->t == DECLARE) {
                
                if (top(stack)->t == MODULE) { //Eg.1 declare module compute;
                    pop(stack);     //Pop module
                    TREENODEPTR n = top(stack);

                    if (findInFunctionList(n->lexeme) == NULL) {
                        //Can be inserted
                        currFuncTable = addFunc(currFuncTable);
                        strcpy(currFuncTable->name, n->lexeme);
                        currFuncTable->status = declared;
                    }
                    else {
                        //ERROR (Re declared)
                    }
                }

                else {                          //Eg.2 declare var_1_, var_2_, var_3_:integer;

                    //handle if id declared is same as function parameter
                    TREENODEPTR _dtype = top(stack)->parent->child[3];
                    TREENODEPTR _idlist = top(stack)->parent->child[1];

                    pop(stack); //IdList popped
                    pop(stack); //COLON popped
                    pop(stack); //DataType popped
                    pop(stack); //SEMICOLON popped

                    int i, count = _idlist->child_count;
                    if (checkinTable(currIdTable, _idlist->child[0]->lexeme) == 1) {
                        //error: already declared
                    }
                    else {
                        //Handle Arrays separately
                        addTupleEntry(currIdTable, _idlist->child[0]->lexeme, _idlist->child[0]->line_num, 0, _dtype->child[0]->t);
                    }
                    TREENODEPTR cur = _idlist->child[1];
                    while(cur->chile[0]->t != e) {

                        TREENODEPTR n = cur->child[1];
                        if (checkinTable(currIdTable, n->lexeme) == 1) {
                            //Error (Already declared)
                        }
                        else {
                            //Handle Arrays separately
                            addTupleEntry(currIdTable, n->lexeme, n->line_num, 0, _dtype->child[0]->t);
                        }
                        cur = cur->child[2];
                    }
                }
            }

            else if (temp->t == START) {
                //Now we have a new Scope

                if (currFuncTable->idTable == NULL) {//The current function has no scope yet
                    currFuncTable->idTable = getNewTable();
                    currIdTable = currFuncTable->idTable;
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
                    TREENODEPTR _dtype = top(stack)->parent->child[2];
                    pop(stack); //colon popped
                    pop(stack); //dataType popped
                    TREENODEPTR cur = top(stack);
                    pop(stack); //ipList2 popped

                    struct Parameters *in_start = (struct Parameters *)malloc(sizeof(struct Parameters));
                    struct Parameters *in = in_start;
                    strcpy(in->name, temp->lexeme);
                    in->type = _dtype->child[0]->t;

                    while(cur->child[0]->t != e){
                        TREENODEPTR n = cur->child[1];
                        _dtype = cur->child[3];
                        if (checkinTable(currIdTable, n->lexeme) == 1) {
                            //Error (Already declared)
                        }
                        else {
                            //Handle Arrays separately
                            
                            in->next = (struct Parameters *)malloc(sizeof(struct Parameters));
                            strcpy(in->next->name, n->lexeme);
                            in->next->type = _dtype->child[0]->t;

                            in = in->next;
                        }
                        cur = cur->child[4];
                    }
                    currFuncTable->in = in_start;
                }

                else if (temp->parent->t == output_plist) {
                    TREENODEPTR _dtype = top(stack)->parent->child[2];
                    pop(stack); //colon popped
                    pop(stack); //dataType popped
                    TREENODEPTR cur = top(stack);
                    pop(stack); //ipList2 popped

                    struct Parameters *out_start = (struct Parameters *)malloc(sizeof(struct Parameters));
                    struct Parameters *out = out_start;
                    strcpy(out->name, temp->lexeme);
                    out->type = _dtype->child[0]->t;

                    while(cur->child[0]->t != e){
                        TREENODEPTR n = cur->child[1];
                        _dtype = cur->child[3];
                        if (checkinTable(currIdTable, n->lexeme) == 1) {
                            //Error (Already declared)
                        }
                        else {
                            //Handle Arrays separately
                            
                            out->next = (struct Parameters *)malloc(sizeof(struct Parameters));
                            strcpy(out->next->name, n->lexeme);
                            out->next->type = _dtype->child[0]->t;

                            out = out->next;
                        }
                        cur = cur->child[4];
                    }
                    currFuncTable->out = out_start;
                }

                else {
                    if (checkIdentifierifDeclared(currIdTable, temp->lexeme) == 0) {
                        //Error
                    }
                }
            }
            else if(temp->t == USE) {
                pop(stack); //module popped
                TREENODEPTR n = top(stack); //ID of function
                if(findInFunctionList(n->lexeme) == NULL){
                    //error
                }
                //Handle if module is never defined
            }
            else if(temp->t == DEF){
                pop(stack); //module popped
                TREENODEPTR n = top(stack); //function id stored
                pop(stack); //function id popped

                struct FuncTable *f = findInFunctionList(n->lexeme);
                if(f){
                    if(f->status == defined){
                        //error: redifining a function
                    }
                    else{
                        currFuncTable = f;
                        currFuncTable->status = defined;

                    }
                }
                else{
                    //add to function list and give memory
                    currFuncTable = addFunc(currFuncTable);
                    strcpy(currFuncTable->name, n->lexeme);
                    currFuncTable->status = defined;

                }
            }

            else if(temp->t == DRIVERDEF){
                currFuncTable = addFunc(currFuncTable);
                strcpy(currFuncTable->name, "driver");
                currFuncTable->status = defined;
            }
        }
    }
}













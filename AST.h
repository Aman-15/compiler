/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include "ASTDef.h"

struct ASTNode *createASTNode();
void addAstChild(struct ASTNode *parent, struct ASTNode *child);
void addIdChild(struct ASTNode *parent, struct IdTuple *child);
void addFuncChild(struct ASTNode *parent, struct FuncTable *child);
void addParaChild(struct ASTNode *parent, struct Parameters *param);
struct ASTNode *generateAST(TREENODEPTR root);

void printAST(struct ASTNode *root, int print);
int getNoOfASTNodes();
int getSizeOfAST();
int getErrAst();
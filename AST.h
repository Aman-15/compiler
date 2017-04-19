// #include "SymbolTable.h"
#include "ASTDef.h"

struct ASTNode *createASTNode();
void addAstChild(struct ASTNode *parent, struct ASTNode *child);
void addIdChild(struct ASTNode *parent, struct IdTuple *child);
void addFuncChild(struct ASTNode *parent, struct FuncTable *child);
void addParaChild(struct ASTNode *parent, struct Parameters *param);
struct ASTNode *generateAST(TREENODEPTR root);

void printAST();
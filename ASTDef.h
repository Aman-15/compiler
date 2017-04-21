/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include "SymbolTable.h"

struct ASTNode;

struct NodeType {
	struct FuncTable *funcEntry;
	struct IdTuple *tupleEntry;
	struct ASTNode *astNode;
	struct Parameters *paraEntry;
};

union ast_value {
    int int_val;
    float float_val;
    enum enum_token bool;
    enum enum_token operator;
};

struct ASTNode {
	// union NodeType nodeType;
	enum enum_token tokenID;
	struct NodeType *child;
	struct ASTNode *parent;
	enum enum_token type;
	union ast_value val;
	int child_count;
};
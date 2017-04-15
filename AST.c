/** Incomplete
1. Rule 32

**/

#include "AST.h"

int max_child = 10;

struct ASTNode *createASTNode(){
	struct ASTNode *newnode = (struct ASTNode *)malloc(sizeof(struct ASTNode));
	newnode->child = (union NodeType *)malloc(max_child * sizeof(union NodeType));
	newnode->parent = NULL;
	newnode->child_count = 0;
	return newnode;
}

void addAstChild(struct ASTNode *parent, struct ASTNode *child){
	parent->child[parent->child_count].astNode = child;
	if(child)
		child->parent = parent;
	parent->child_count++;
}
void addIdChild(struct ASTNode *parent, struct IdTuple *child){
	parent->child[parent->child_count].tupleEntry = child;
	parent->child_count++;
}
void addFuncChild(struct ASTNode *parent, struct FuncTable *child){
	parent->child[parent->child_count].funcEntry = child;
	parent->child_count++;
}
void addParaChild(struct ASTNode *parent, struct Parameters *param){
	parent->child[parent->child_count].paraEntry = param;
	parent->child_count++;
}
struct ASTNode *generateAST(TREENODEPTR root){
	struct ASTNode *ast = createASTNode();
	if(root->t_or_nt == 0){
		ast->tokenID = root->t;
		switch(root->t){

			case program: //Rule 1
				// ast->tokenID = program;
				for(int i = 0; i < root->child_count; i++){
					addAstChild(ast, generateAST(root->child[i]));
				}
				// return ast;
			break;
			
			case moduleDeclarations: //Rule 2
				// ast->tokenID = moduleDeclarations;
				if(root->child[0]->t == moduleDeclaration){
					for(int i = 0; i < root->child_count; i++){
						addAstChild(ast, generateAST(root->child[i]));
					}
					// return ast;	
				}
				else {//it gives e Rule 3
					free(ast);
					return NULL;
				}
			break;
			
			case otherModules: //Rule 5
				// ast->tokenID = otherModules;
				if(root->child[0]->t == module){
					for(int i = 0; i < root->child_count; i++){
						addAstChild(ast, generateAST(root->child[i]));
					}
					// return ast;
				} 
				else {//it gives e //Rule 6
					free(ast);
					return NULL;
				}
			break;
			
			case driverModule: //Rule 7
				// ast->tokenID = driverModule;
				addAstChild(ast, generateAST(root->child[4]));
				// return ast;
			break;
			
			case moduleDef: //Rule 24
				// ast->tokenID = moduleDef;
				free(ast);
				return generateAST(root->child[1]);
			break;
			
			case statements: //Rule 25
				// ast->tokenID = statements;
				if(root->child[0]->t == statement){
					for(int i = 0; i < root->child_count; i++){
						addAstChild(ast, generateAST(root->child[i]));
					}
					// return ast;
				}
				else{//it gives e Rule 26
					free(ast);
					return NULL;
				}
			break;
			
			case moduleDeclaration: //Rule 4
				addFuncChild(ast, findInFunctionList(root->child[2]->lexeme));
				// return ast;
			break;
			
			case module: //Rule 8
				addFuncChild(ast, findInFunctionList(root->child[2]->lexeme));
				addAstChild(ast, generateAST(root->child[7]));
				addAstChild(ast, generateAST(root->child[10]));
				addAstChild(ast, generateAST(root->child[11]));
				// return ast;
			break;
			
			case ret: //Rule 9
				if (root->child[0]->t != e)
					addAstChild(ast, generateAST(root->child[2]));
				else {	//Rule 10
					free(ast);
					return NULL;
				}
			break;
			
			case input_plist: //Rule 11
				addParaChild(ast, findInFunctionList(root->parent->child[2]->lexeme)->in);
			break;	//Rule 12, Rule 13 ignored
			
			case output_plist:	//Rule 14
				addParaChild(ast, findInFunctionList(root->parent->parent->child[2]->lexeme)->out);
			break;	//Rule 15, Rule 16 ignored

			case dataType:	//Rule 17, 18, 19
				if (root->child[0]->t != ARRAY)
					ast->type = root->child[0]->t;
				else {	//Rule 20
					ast->type = ARRAY;
					addAstChild(ast, generateAST(root->child[2]));
					addAstChild(ast, generateAST(root->child[5]));
				}
			break;

			case type: //Rule 21, 22, 23
				ast->type = root->child[0]->t;
			break;

			case statement:	//Rule 27 - 31
				free(ast);
				return generateAST(root->child[0]);
			break;

			case ioStmt: //Rule 32
				if (root->child[0]->t == GET_VALUE) {
					//Return with scope

				}
				else { //Rule 33
					free(ast);
					return generateAST(root->child[2]);
				}
			break;

			case var:
				switch(root->child[0]->t) {
					case ID:	//Rule 34
						//Incomplete
						addIdChild(ast, root->child[0]->tuple);
						if (root->child[0]->tuple)
							ast->type = root->child[0]->tuple->type;
						if(root->child[1]->child[0]->t != e){
							addIdChild(ast, root->child[1]->child[1]->tuple);	
						}
						else
							addIdChild(ast, NULL);	
					break;

					case NUM:	//Rule 35
						ast->type = INTEGER;
						ast->val.int_val = root->child[0]->val.int_val;
					break;

					case RNUM:	//Rule 36
						ast->type = REAL;
						ast->val.int_val = root->child[0]->val.int_val;
					break;

					case TRUE:	//Rule 37
						ast->type = BOOLEAN;
						ast->val.bool = TRUE;
					break;

					case FALSE:	//Rule 38
						ast->type = INTEGER;
						ast->val.bool = FALSE;
					break;

					default:
						printf("This is dfault of nested switch\n");
					break;
				}
			break;

			case whichId:	//Rule 39
				if (root->child[0]->t != e) {
					//Incomplete
					printf("In whichId, should never reach here\n");
				}
				else {	//Rule 40
					printf("In whichId, should never reach here\n");
					free(ast);
					return NULL;
				}

			break;

			case simpleStmt: //Rule 41, Rule 42
				free(ast);
				return generateAST(root->child[0]);
			break;

			case assignmentStmt:	//Rule 43
				//Incomplete
				;struct ASTNode *ws = generateAST(root->child[1]);
				//printf("Line num before seg fault: %d\n", root->line_num);
				
				addIdChild(ast, root->child[0]->tuple);
				addAstChild(ast, generateAST(root->child[1]));
				if(root->child[0]->tuple && ws->type != root->child[0]->tuple->type){
					printf("line:%d Type mismatch while assigning\n", root->child[0]->line_num);
						return NULL;
				}
			break;

			case whichStmt:		//Rule 44, Rule 45
				free(ast);
				return generateAST(root->child[0]);
			break;

			case lvalueIDStmt:	//Rule 46
				free(ast);
				return generateAST(root->child[1]);
			break;

			case lvalueARRStmt:	//Rule 47
				;
				struct ASTNode *expr = generateAST(root->child[4]);
				ast->type = expr->type;
				if(root->child[1]->child[0]->t == NUM){
					addAstChild(ast, generateAST(root->child[1]));
					addAstChild(ast, expr);
				}
				else{// Rule 49
					addIdChild(ast, root->child[1]->child[0]->tuple);
					addAstChild(ast, expr);
				}
			break;

			case index1:
				if (root->child[0]->t == NUM) {	//Rule 48
					ast->type = INTEGER;
					ast->val.int_val = root->child[0]->val.int_val;
				}
				else {	//Rule 49
					//Incomplete

				}
			break;

			case moduleReuseStmt:	//Rule 50
				addAstChild(ast, generateAST(root->child[0]));
				addFuncChild(ast, findInFunctionList(root->child[3]->lexeme));
				addAstChild(ast, generateAST(root->child[6]));
			break;

			case optional:	//Rule 51, 52
				free(ast);
				if (root->child[0]->t != e)
					return generateAST(root->child[1]);
				return NULL;
			break;

			case idList:	//Rule 53
				//Incomplete
				addIdChild(ast, root->child[0]->tuple);
				addAstChild(ast, generateAST(root->child[1]));
			break;

			case idList2: //Rule 54, 55
				if(root->child[0]->t != e){
					addIdChild(ast, root->child[1]->tuple);
					addAstChild(ast, generateAST(root->child[2]));
				}
				else {
					free(ast);
					return NULL;
				}
			break;

			case expression:
				free(ast);
				if (root->child[0]->t == MINUS) { //Rule 57
					struct ASTNode *bonbe = generateAST(root->child[1]);
					if (bonbe->type == BOOLEAN) {
						printf("line:%d Cannot be a boolean expression after MINUS\n", root->child[0]->line_num);
						return NULL;
					}
					else
						return bonbe;
				}
				else	//Rule 56
					return generateAST(root->child[0]);

			break;

			case booleanOrNonBooleanArithmeticExpr: //Rule 58
				free(ast);
				if (root->child[0]->t == BO)
					return generateAST(root->child[1]);
				return generateAST(root->child[0]); //Rule 59
			break;

			case arithmeticOrBooleanExpr:	//Rule 60

				if (root->child[0]->t != BO) {
					struct ASTNode *at = generateAST(root->child[0]);
					struct ASTNode *ewlo = generateAST(root->child[1]);

					if (ewlo != NULL)
						ast->type = BOOLEAN;
					else
						ast->type = at->type;

					addAstChild(ast, at);
					addAstChild(ast, ewlo);
				}
				else {	//Rule 61
					struct ASTNode *aorbe = generateAST(root->child[1]);
					ast->type = aorbe->type;
					addAstChild(ast, aorbe);
					addAstChild(ast, generateAST(root->child[3]));
				}
			break;

			case arithmeticOrBooleanExpr2:	//Rule 62, 63, 64, 65
				if (root->child[0]->t != e) {
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, generateAST(root->child[1]));
				}
				else {	//Rule 66
					free(ast);
					return NULL;
				}
			break;

			case expressionWithLogOp:	//Rule 67
				if (root->child[0]->t != e) {
					struct ASTNode *at2 = generateAST(root->child[1]);
					if (at2->type != BOOLEAN) {
						printf("line:%d Cannot be a non-boolean expression\n", root->child[1]->child[0]->line_num);
						return NULL;
					}
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, at2);
					addAstChild(ast, generateAST(root->child[2]));
				}
				else {	//Rule 68
					free(ast);
					return NULL;
				}
			break;

			case anyTerm:	//rule 69
				;struct ASTNode *ewro = generateAST(root->child[1]);
				;struct ASTNode *ae = generateAST(root->child[0]);
				if (ewro != NULL)
					ast->type = BOOLEAN;
				else
					ast->type = ae->type;
				addAstChild(ast, ae);
				addAstChild(ast, ewro);
			break;

			case anyTerm2: //Rule 70
				free(ast);
				if (root->child[0]->t == BO) {
					return generateAST(root->child[1]);
				}
				else	//Rule 71
					return generateAST(root->child[0]);
			break;

			case expressionWithRelOp:	//Rule 72
				if (root->child[0]->t != e) {
					struct ASTNode *nopae = generateAST(root->child[1]);
					if (nopae->type != BOOLEAN) {
						printf("line:%d Cannot be a non-boolean expression\n", root->line_num);
						return NULL;
					}
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, nopae);
					addAstChild(ast, generateAST(root->child[2]));
				}
				else {	//Rule 73
					free(ast);
					return NULL;
				}
			break;

			case negOrPosArithmeticExpr:
				if (root->child[0]->t == MINUS) {	//Rule 74
					struct ASTNode *bonbe1 = generateAST(root->child[1]);
					if (bonbe1->type == BOOLEAN) {
						printf("line:%d Cannot be a boolean expression after MINUS\n", root->child[0]->line_num);
						return NULL;
					}
					else
						return bonbe1;
				}
				else {	//Rule 75
					free(ast);
					return generateAST(root->child[0]);
				}
			break;

			case arithmeticExpr:	//Rule 76
				;struct ASTNode *trm = generateAST(root->child[0]);
				ast->type = trm->type;
				addAstChild(ast, trm);
				addAstChild(ast, generateAST(root->child[1]));
			break;

			case arithmeticExpr2:
				if (root->child[0]->t != e) {	//Rule 77
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, generateAST(root->child[1]));
					addAstChild(ast, generateAST(root->child[2]));
				}
				else {	//Rule 78
					free(ast);
					return NULL;
				}
			break;

			case term:	//Rule 79
				;struct ASTNode *fctr = generateAST(root->child[0]);
				ast->type = fctr->type;
				addAstChild(ast, fctr);
				addAstChild(ast, generateAST(root->child[1]));
			break;

			case term2:
				if (root->child[0]->t != e) {	//Rule 80
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, generateAST(root->child[1]));
					addAstChild(ast, generateAST(root->child[2]));
				}
				else {	//Rule 81
					free(ast);
					return NULL;
				}
			break;

			case factor:	//Rule 82
				free(ast);
				return generateAST(root->child[0]);
			break;

			case op1:	//Rule 83, 84
				ast->val.operator = root->t;
			break;

			case op2:	//Rule 85, 86
				ast->val.operator = root->t;
			break;

			case logicalOp:	//Rule 87, 88
				ast->val.operator = root->t;
			break;

			case relationalOp:	//Rule 89 - 94
				ast->val.operator = root->t;
			break;

			case declareStmt:	//Rule 95
				;struct ASTNode *dt = generateAST(root->child[3]);
				;struct ASTNode *idlst = generateAST(root->child[1]);
				idlst->type = dt->type;
				addAstChild(ast, idlst);
				addAstChild(ast, dt);
			break;

			case conditionalStmt:	//Rule 96
				addAstChild(ast, generateAST(root->child[5]));
				addAstChild(ast, generateAST(root->child[6]));
			break;

			case caseStmts:		//Rule 97
				addAstChild(ast, generateAST(root->child[1]));
				addAstChild(ast, generateAST(root->child[3]));
				addAstChild(ast, generateAST(root->child[6]));
			break;

			case caseStmt:		//Rule 98, 99
				if (root->child[0]->t != e) {
					addAstChild(ast, generateAST(root->child[1]));
					addAstChild(ast, generateAST(root->child[3]));
					addAstChild(ast, generateAST(root->child[6]));
				}
				else {
					free(ast);
					return NULL;
				}
			break;

			case value:	//Rule 100
				if (root->child[0]->t == NUM) {
					ast->type = INTEGER;
					ast->val.int_val = root->child[0]->val.int_val;
				}
				else {	//Rule 101, 102
					ast->type = BOOLEAN;
					ast->val.bool = root->child[0]->t;
				}
			break;

			case default1:	//Rule 103, 104
				if (root->child[0]->t != e)
					return generateAST(root->child[2]);
				free(ast);
				return NULL;
			break;

			case iterativeStmt:	//Rule 105
				//Incomplete
				if (root->child[0]->t != FOR) {	//Rule 106
					struct ASTNode *aobe2 = generateAST(root->child[2]);
					if (aobe2->type != BOOLEAN) {
						printf("line:%d While condition must be a boolean expression\n", root->child[0]->line_num);
						return NULL;
					}
					addAstChild(ast, aobe2);
					addAstChild(ast, generateAST(root->child[5]));
				}
				else{ //Rule 105
					addIdChild(ast, root->child[2]->tuple);
					addAstChild(ast, generateAST(root->child[4]));
					addAstChild(ast, generateAST(root->child[7]));
				}
			break;

			case range:	//Rule 107
				;struct ASTNode *num1 = createASTNode();
				;struct ASTNode *num2 = createASTNode();
				num1->type = num2->type = INTEGER;
				num1->val.int_val = root->child[0]->val.int_val;
				num2->val.int_val = root->child[2]->val.int_val;
				num1->tokenID = num2->tokenID = NUM;
				addAstChild(ast, num1);
				addAstChild(ast, num2);
			break;

			default:
				printf("This is default\n");
			break;
		}//End of Switch

		return ast;
	}
	printf("Outside switch case\n");
	return NULL;
}
/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/

#include "AST.h" 

char *string_tokens2[] = {
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

int max_child = 5;
int errAst = 0;

struct ASTNode *createASTNode(){
	struct ASTNode *newnode = (struct ASTNode *)malloc(sizeof(struct ASTNode));
	newnode->child = (struct NodeType *)malloc(max_child * sizeof(struct NodeType));
	int i=0;
	while (i < max_child) {
		newnode->child[i].funcEntry = NULL;
		newnode->child[i].paraEntry = NULL;
		newnode->child[i].astNode = NULL;
		newnode->child[i].tupleEntry = NULL;
		i++;
	}
	
	newnode->parent = NULL;
	newnode->child_count = 0;
	newnode->tokenID = 0;
	newnode->type = 0;
	newnode->val.float_val = 0;
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
		int i;
		switch(root->t){

			case program: //Rule 1
				// ast->tokenID = program;
				for(i = 0; i < root->child_count; i++){
					addAstChild(ast, generateAST(root->child[i]));
				}
				// return ast;
			break;
			
			case moduleDeclarations: //Rule 2
				// ast->tokenID = moduleDeclarations;
				if(root->child[0]->t == moduleDeclaration){
					for(i = 0; i < root->child_count; i++){
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
					for(i = 0; i < root->child_count; i++){
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
					//printf("%d \n", root->child_count);
					for(i = 0; i < root->child_count; i++){
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
					addIdChild(ast, root->child[2]->tuple);
				}
				else { //Rule 33
					free(ast);
					return generateAST(root->child[2]);
				}
			break;

			case var:
				switch(root->child[0]->t) {
					case ID:	//Rule 34
						addIdChild(ast, root->child[0]->tuple);
						if (root->child[0]->tuple) {
							ast->type = root->child[0]->tuple->type;
						}
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
						printf("This is default of nested switch\n");
					break;
				}
			break;

			case whichId:	//Rule 39
				if (root->child[0]->t != e) {
					printf("In whichId, should never reach here\n");
				}
				else {	//Rule 40
					printf("In whichId, should never reach here\n");
				}
			break;

			case simpleStmt: //Rule 41, Rule 42
				free(ast);
				return generateAST(root->child[0]);
			break;

			case assignmentStmt:	//Rule 43
				;struct ASTNode *ws = generateAST(root->child[1]);
				
				addIdChild(ast, root->child[0]->tuple);
				
				if(root->child[0]->tuple && ws->type != root->child[0]->tuple->type){
					if (ws->type != 0) {
						errAst = 1;
						printf("line:%d Type mismatch while assigning '%s' and '%s'\n", root->child[0]->line_num, string_tokens2[ws->type], string_tokens2[root->child[0]->tuple->type]);
					}
					free(ast);
					return NULL;
				}
				addAstChild(ast, generateAST(root->child[1]));
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
					free(ast);
					errAst = 1;
					printf("line:%d Index must be a variable\n", root->child[1]->child[0]->line_num);
				}
				else{// Rule 49
					if (root->child[1]->child[0]->tuple && root->child[1]->child[0]->tuple->type != INTEGER) {
						free(ast);
						errAst = 1;
						printf("line:%d Index must be an integer\n", root->child[1]->child[0]->line_num);
					}
					else {
						addIdChild(ast, root->child[1]->child[0]->tuple);
						addAstChild(ast, expr);
					}
				}
			break;

			case index1:
				//Rule 48 and Rule 49 handled above
			break;

			case moduleReuseStmt:	//Rule 50
				;struct FuncTable* func = findInFunctionList(root->child[3]->lexeme);
				addAstChild(ast, generateAST(root->child[0]));
				addFuncChild(ast, func);
				addAstChild(ast, generateAST(root->child[6]));
				;struct Parameters *in = func->in;
				;struct Parameters *out = func->out;
				
				;struct ASTNode *in_list = ast->child[2].astNode;
				;struct ASTNode *out_list = ast->child[0].astNode;

				if(func->status == declared) {
					printf("Line: %d Error: Used function is never defined\n", root->child[1]->line_num);
						errAst = 1;
				}

				if(in && in_list) {
					if(in_list->child[0].tupleEntry && in->type != in_list->child[0].tupleEntry->type){
						errAst = 1;
						printf("1 Line: %d Error in input Parameters, Expected %s but found %s\n", root->child[1]->line_num, string_tokens2[in->type], string_tokens2[in_list->child[0].tupleEntry->type]);
					}
					in = in->next;
					in_list = in_list->child[1].astNode;
					while(in && in_list){
						if(in_list->child[0].tupleEntry && in->type != in_list->child[0].tupleEntry->type){

						errAst = 1;
							printf("2 Line: %d Error in input Parameters, Expected %s but found %s\n", root->child[1]->line_num, string_tokens2[in->type], string_tokens2[in_list->child[0].tupleEntry->type]);
						}
						in = in->next;
						in_list = in_list->child[1].astNode;
					}
				}
				if(in){
						errAst = 1;
					printf("Line: %d More input Parameters Expected\n", root->child[1]->line_num);
				}
				if(in_list){
						errAst = 1;
					printf("Line: %d More input Parameters supplied\n", root->child[1]->line_num);
				}

				//Checking type of output_plist
				if(out && out_list){
					if(out_list->child[0].tupleEntry && out->type != out_list->child[0].tupleEntry->type){
						errAst = 1;
						printf("1 Line: %d Error in output Parameters, Expected %s but found %s\n", root->child[1]->line_num, string_tokens2[out->type], string_tokens2[out_list->child[0].tupleEntry->type]);
					}
					out = out->next;
					out_list = out_list->child[1].astNode;
					while(out && out_list){
						if(out_list->child[0].tupleEntry && out->type != out_list->child[0].tupleEntry->type){

						errAst = 1;
							printf("2 Line: %d Error in output Parameters, Expected %s but found %s\n", root->child[1]->line_num, string_tokens2[out->type], string_tokens2[out_list->child[0].tupleEntry->type]);
						}
						out = out->next;
						out_list = out_list->child[1].astNode;
					}
				}
				if(out){
						errAst = 1;
					printf("Line: %d More output Parameters Expected\n", root->child[1]->line_num);
				}
				if(out_list){
						errAst = 1;
					printf("Line: %d More output Parameters supplied\n", root->child[1]->line_num);
				}
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
						errAst = 1;
						printf("line:%d Cannot be a boolean expression after MINUS\n", root->child[0]->line_num);
						free(ast);
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

					if (ewlo != NULL) {
						if (at->type != BOOLEAN) {
						errAst = 1;
							printf("line:%d Expected a BOOLEAN expression\n", root->child[1]->child[0]->child[0]->line_num);
						}
						ast->type = BOOLEAN;
					}
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
					struct ASTNode *ewlp2 = generateAST(root->child[2]);

					if (at2->type != BOOLEAN) {	
						errAst = 1;
						printf("line:%d Cannot be a non-boolean expression\n", root->child[0]->child[0]->line_num);
						at2->type = BOOLEAN;
					}
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, at2);
					addAstChild(ast, ewlp2);
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
				else if (ae)
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
					addAstChild(ast, generateAST(root->child[0]));
					addAstChild(ast, nopae);
					addAstChild(ast, generateAST(root->child[2]));
					if (nopae->type == BOOLEAN) {
						errAst = 1;
						printf("line:%d Expression must be either INTEGER or REAL\n", root->child[0]->child[0]->line_num);
						free(ast);
						return NULL;
					}
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
						errAst = 1;
						printf("line:%d Cannot be a boolean expression after MINUS\n", root->child[0]->line_num);
						free(ast);
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
				;struct ASTNode *ae2 = generateAST(root->child[1]);
				if (ae2 && ae2->type != trm->type) {
					if (ae2->type && trm->type) {
						printf("line:%d Type mismatch in expression '%s' and '%s'\n", root->child[1]->child[0]->child[0]->line_num,
							string_tokens2[trm->type], string_tokens2[ae2->type]);
						errAst = 1;
					}
					free(ast);
					return NULL;
				}

				addAstChild(ast, trm);
				addAstChild(ast, ae2);

				if (trm->type == BOOLEAN && ae2) {
						errAst = 1;
					printf("line:%d Cannot be a boolean expression\n", root->child[0]->child[0]->child[0]->child[0]->line_num);
					ast->type = 0;
				}
				else
					ast->type = trm->type;
			break;

			case arithmeticExpr2:
				if (root->child[0]->t != e) {	//Rule 77
					addAstChild(ast, generateAST(root->child[0]));
					;struct ASTNode *trm2 = generateAST(root->child[1]);
					;struct ASTNode *ae22 = generateAST(root->child[2]);

					if (ae22 && trm2 && ae22->type != trm2->type) {
						if (ae22->type && trm2->type) {
							printf("line:%d Type mismatch in expression '%s' and '%s'\n", root->child[0]->child[0]->line_num,
								string_tokens2[trm2->type], string_tokens2[ae22->type]);
						errAst = 1;
						}
						free(ast);
						return NULL;
					}
					addAstChild(ast, trm2);
					addAstChild(ast, ae22);
					ast->type = trm2->type;
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
				;struct ASTNode *cstmts = generateAST(root->child[5]);
				;struct ASTNode *dflt = generateAST(root->child[6]);
				if (root->child[2]->tuple->type == REAL) {
						errAst = 1;
					printf("line:%d Switch condition must be a INTEGER or BOOLEAN\n", root->child[0]->line_num);
				}
				if(root->child[2]->tuple->type == BOOLEAN){
					if(cstmts->child[0].astNode->type != BOOLEAN){
						errAst = 1;
						printf("Line: %d expected value here is of type boolean\n", root->child[5]->child[0]->line_num);
					}
					;struct ASTNode *cstmt = cstmts->child[2].astNode;
					TREENODEPTR csmt_pt = root->child[5]->child[6];
					while(cstmt != NULL){
						if(cstmt->child[0].astNode->type != BOOLEAN){
						errAst = 1;
							printf("Line: %d expected value here is of type boolean\n", csmt_pt->child[0]->line_num);
						}
						csmt_pt = csmt_pt->child[6];
						cstmt = cstmt->child[2].astNode;	
					}
				}
				else if(root->child[2]->tuple->type == INTEGER){
					if(cstmts->child[0].astNode->type != INTEGER){
						errAst = 1;
						printf("Line: %d expected value here is of type INTEGER\n", root->child[5]->child[0]->line_num);
					}
					;struct ASTNode *cstmt = cstmts->child[2].astNode;
					TREENODEPTR csmt_pt = root->child[5]->child[6];
					while(cstmt != NULL){
						if(cstmt->child[0].astNode->type != INTEGER){
						errAst = 1;
							printf("Line: %d expected value here is of type INTEGER\n", csmt_pt->child[0]->line_num);
						}
						csmt_pt = csmt_pt->child[6];
						cstmt = cstmt->child[2].astNode;	
					}
				}
				
				if (root->child[2]->tuple->type == INTEGER && dflt == NULL) {
						errAst = 1;
					printf("line:%d Switch must have a \"default\" case if it is an integer\n", root->child[0]->line_num);
				}
				else if (root->child[2]->tuple->type == BOOLEAN && dflt != NULL) {
						errAst = 1;
					printf("line:%d Switch must not have a \"default\" case if it is a boolean\n", root->child[0]->line_num);
				}
				addAstChild(ast, cstmts);
				addAstChild(ast, dflt);
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
				else if (root->child[0]->t == TRUE || root->child[0]->t == FALSE) {	//Rule 101, 102
					ast->type = BOOLEAN;
					ast->val.bool = root->child[0]->t;
				}
				else {
						errAst = 1;
					printf("line:%d Case value must be a BOOLEAN or INTEGER\n", root->child[0]->line_num);
				}
			break;

			case default1:	//Rule 103, 104
				if (root->child[0]->t != e)
					return generateAST(root->child[2]);
				free(ast);
				return NULL;
			break;

			case iterativeStmt:
				if (root->child[0]->t != FOR) {	//Rule 106
					struct ASTNode *aobe2 = generateAST(root->child[2]);
					if (aobe2->type != BOOLEAN) {
						errAst = 1;
						printf("line:%d While condition must be a boolean expression\n", root->child[0]->line_num);
						aobe2->type = BOOLEAN;
					}
					addAstChild(ast, aobe2);
					addAstChild(ast, generateAST(root->child[5]));
				}
				else{ //Rule 105
					if (root->child[2]->tuple->type != INTEGER) {
						errAst = 1;
						printf("line:%d Iterator must be an integer\n", root->child[2]->line_num);
					}
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

int noOfASTNodes = 0;
int sizeOfAST = 0;

void printAST(struct ASTNode *root, int print) {
	if (print)
		printf("%20s\tNon Terminal\n",string_tokens2[count_terminal + root->tokenID]);
	noOfASTNodes++;
	sizeOfAST += sizeof(root);

	if (root->child) {
		if (root->child[0].astNode)
			printAST(root->child[0].astNode, print);
		else if (root->child[0].funcEntry) {
			if (print)
				printf("%20s\tSymbol Table Entry\n", root->child[0].funcEntry->name);
			noOfASTNodes++;
			sizeOfAST += sizeof(root->child[0].funcEntry);
		}
		else if (root->child[0].tupleEntry) {
			if (print)
				printf("%20s\tSymbol Table Entry\n", root->child[0].tupleEntry->name);
			noOfASTNodes++;
			sizeOfAST += sizeof(root->child[0].tupleEntry);
		}
		else if (root->child[0].tupleEntry) {
			if (print)
				printf("%20s\tSymbol Table Entry\n", root->child[0].paraEntry->name);
			noOfASTNodes++;
			sizeOfAST += sizeof(root->child[0].paraEntry);
		}
	}

	int i;
	for (i=1; i<root->child_count; i++) {
		if (root->child[i].astNode) 
			printAST(root->child[i].astNode, print);
		else if (root->child[i].funcEntry) {
			if (print)
				printf("%20s\tSymbol Table Entry\n", root->child[i].funcEntry->name);
			noOfASTNodes++;
			sizeOfAST += sizeof(root->child[i].funcEntry);
		}
		else if (root->child[i].tupleEntry) {
			if (print)
				printf("%20s\tSymbol Table Entry\n", root->child[i].tupleEntry->name);
			noOfASTNodes++;
			sizeOfAST += sizeof(root->child[i].tupleEntry);
		}
		else if (root->child[i].tupleEntry) {
			if (print)
				printf("%20s\tSymbol Table Entry\n", root->child[i].paraEntry->name);
			noOfASTNodes++;
			sizeOfAST += sizeof(root->child[i].paraEntry);
		}
	}
}

int getNoOfASTNodes() {
	return noOfASTNodes;
}

int getSizeOfAST() {
	return sizeOfAST;
}

int getErrAst() {
	return errAst;
}
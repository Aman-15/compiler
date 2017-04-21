/*
BATCH NUMBER 75
AMAN AGARWAL 2014A7PS042P
MANIK BHANDARI 2014A7PS088P
*/
#include "codegen.h"

int labelStack[100];
char switchStack[10][100];
int stackTop=-1;
int switchTop=-1;
int label = 1;

void pushLabel(int a) {
	labelStack[++stackTop] = a;
}

int popLabel() {
	if (stackTop == -1) {
		return -1; 
	}
	return labelStack[--stackTop];
}

int topLabel() {
	if (stackTop != -1) {
		return labelStack[stackTop];
	}
	return 0;
}

void pushSwitch(char *a) {
	strcpy(switchStack[++switchTop], a);
}

void popSwitch() {
	if (switchTop == -1) {
		return; 
	}
	--switchTop;
}

void printAssemblyCode(TREENODEPTR root)
{
	FILE *fp=fopen("code.asm","w");
	fprintf(fp, "global main\n" );
	fprintf(fp, "extern printf\n" );
	fprintf(fp, "extern scanf\n" );
	fprintf(fp, "extern puts\n" );
	fprintf(fp, "section .data\n" );
	fprintf(fp, "base: times 5000 db 0\n" );
	char str[]="%d";
	fprintf(fp, "MEMPRINT: db \"a=%s\", 10, 0 \n",str);
	fprintf(fp,"OUTPUT: db \"Input: %s\",0\n",str);
    fprintf(fp,"INPUT: times 4 db 0\n");	
	fprintf(fp, "section .text\n\n" );
	fprintf(fp, "main:\n" );
	fprintf(fp, "MOV ESI,0\n");

	generateCode(root, fp);
}
void generateCode(TREENODEPTR root, FILE *fp){
	int i;
	if(!root) return;
	for(i = 0; i < root->child_count; i++){
		generateCode(root->child[i], fp);
	}

	if (root->t == PRINT && root->t_or_nt)
	{
		if(root->parent->child[2]->child[0]->tuple->arr_or_not ==0)
		{
			int offset = root->parent->child[2]->child[0]->tuple->offset;
			fprintf(fp, "MOV RAX,0\n");
			fprintf(fp, "MOV AX,word[base+%d]\n",offset );
			fprintf(fp, "CMP AX,0\n");
			fprintf(fp, "MOV RBX,0xFFFFFFFFFFFFFFFF\n");
			fprintf(fp, "CMOVL RAX,RBX\n");
			fprintf(fp, "MOV AX,word[base+%d]\n",offset );
			fprintf(fp, "PUSH RAX\n");
			fprintf(fp, "PUSH RCX\n");
			fprintf(fp, "MOV RDI,MEMPRINT\n");
			fprintf(fp, "MOV RSI,RAX\n");
			fprintf(fp, "XOR RAX,RAX\n");
			fprintf(fp, "call printf\n");
			fprintf(fp, "POP RCX\n");
			fprintf(fp, "POP RAX\n");
		}
		else{
			//HANDLE arrays separately
		}
	}
	else if(root->t == GET_VALUE  && root->t_or_nt){
		int offset = root->parent->child[2]->tuple->offset;;
		fprintf(fp, "LEA RDI, [OUTPUT]\n" );
		fprintf(fp, "LEA RSI, [INPUT] \n" );
		fprintf(fp, "XOR EAX, EAX\n" );
		fprintf(fp, "call scanf \n" );
		fprintf(fp, "MOV EAX, [INPUT]\n" );
		fprintf(fp, "MOV ESI, 0\n" );
		fprintf(fp, "MOV SI, 2\n" );
		fprintf(fp, "DEC SI\n");
		fprintf(fp, "DEC SI\n");
		fprintf(fp, "MOV [base+%d], AX\n",offset);		
	}
	else if(root->t == ASSIGNOP && root->t_or_nt){
		if(root->parent->t == lvalueIDStmt){
			int offset = root->parent->parent->parent->child[0]->tuple->offset; //lvalueid
			TREENODEPTR temp = root->parent->child[1]->child[0]->child[0]->child[0]->child[0]->child[0]->child[0]->child[0];
			if(temp->t == NUM){
				fprintf(fp, "MOV AX,%d\n", temp->val.int_val);	
				fprintf(fp,"MOV word[base+%d],AX\n",offset);
			}
			else if(temp->t == ID && root->t_or_nt){
				if(temp->tuple->arr_or_not == 0){
					fprintf(fp, "MOV AX, word[base+%d]\n", temp->tuple->offset);	
					fprintf(fp,"MOV word[base+%d],AX\n",offset);			
				}
				else{
					//HANDLE arrays separately
				}
			}
			
		}
		else{
			//HANDLE arrays separately
		}
	}
	else if (root->t == SWITCH && root->t_or_nt) {
		fprintf(fp, "MOV AX, word[base+%d]\n", root->parent->child[2]->tuple->offset);
	}
	else if (root->t == CASE && root->t_or_nt) {
		fprintf(fp,"label%d_entry: \n",label);
		pushLabel(label);
		label++;

		if (root->parent->child[1]->child[0]->t == TRUE) {
			fprintf(fp, "MOV BX,1\n");
		}
		else if (root->parent->child[1]->child[0]->t == FALSE) {
			fprintf(fp, "MOV BX,0\n");
		}
		else if (root->parent->child[1]->child[0]->t == NUM) {
			fprintf(fp, "MOV BX,%d\n", root->parent->child[1]->child[0]->val.int_val);
		}

		fprintf(fp, "CMP AX, BX\n");
		fprintf(fp, "JNE label%d_exit\n", topLabel());
	}
	else if (root->t == DEFAULT && root->t_or_nt) {
		fprintf(fp, "label%d_entry: \n", label);
		pushLabel(label);
		label++;
	}
	else if (root->t == BREAK && root->t_or_nt) {
		fprintf(fp, "JMP switch%d_exit\n", switchTop+1);
		fprintf(fp, "label%d_exit: \n", topLabel());
		popLabel();
	}
	else if (root->t == END && root->t_or_nt) {
		if (root->parent->t == conditionalStmt) {
			fprintf(fp, "switch%d_exit: \n", switchTop+1);
			popSwitch();
		}
		else if (root->parent->t == iterativeStmt) {
			if (root->parent->child[0]->t == FOR) {
				int offset = root->parent->child[2]->tuple->offset;
				fprintf(fp, "MOV AX, word[base+%d]\n", offset);
				fprintf(fp, "INC AX\n" );
				fprintf(fp, "MOV word[base+%d],AX\n", offset);
				fprintf(fp, "jmp label%d_entry\n", topLabel());
				fprintf(fp, "label%d_exit:\n", topLabel());
				popLabel();
			}
		}
	}
	else if (root->t == START && root->t_or_nt) {
		if (root->parent->t == iterativeStmt) {
			if (root->parent->child[0]->t == FOR) {
				int offset = root->parent->child[2]->tuple->offset;
				int last = root->parent->child[4]->child[2]->val.int_val;
				fprintf(fp, "label%d_entry: \n", label);
				pushLabel(label);
				label++;
				fprintf(fp, "MOV AX,%d \n", last);
				fprintf(fp, "CMP word[base+%d], AX\n", offset);
				fprintf(fp, "JG label%d_exit\n", topLabel());
			}
		}
	}
}

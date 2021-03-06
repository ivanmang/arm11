/////////////////////////////////////////////////////////////////////////////////////////
//C Group Project - First Year (2016-2017)
//File.assemble.h
//Group: 6
//Member: Cheung, Ka (klc116), Mang, Hao (hxm16), Cheuk, Ki (kfc216), Chan, Chun (ckc116)
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef ARM11_06_ASSEMBLE_H
#define ARM11_06_ASSEMBLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <string.h>
#include "tokenizer.h"
#include "../emulator/instruction.h"

#define MAX_NUMBER_OF_LINES 100
#define LShiftL(x, n)     ((x) << (n))
#define RotateR(x, n)     ((x >> n) | LShiftL(x, 32 - n))
#define NOT_EXIST 0xffffffffu

/////////////////////////////////////////////////////////////////////////////////////////
//  Linked List Abstract Data Type
/////////////////////////////////////////////////////////////////////////////////////////
struct Linkedlist{
    struct Linkedlist *next;
    u32 value;
    char *key;
}*head;

struct Linkedlist* getNewlist(void);
u32 lookUpValue(struct Linkedlist *list, char *key);
void printLinkedList(struct Linkedlist* linkedlist);
void insertElement(struct Linkedlist **list, char *key, u32 value);
/////////////////////////////////////////////////////////////////////////////////////////
//  Binary Search Tree Abstract Data Type
/////////////////////////////////////////////////////////////////////////////////////////
typedef int (*bst_compare_t)(void*, void*);

typedef struct BST_node{
    char * key;
    void * value;
    struct BST_node* left;
    struct BST_node* right;
}BST_node;


typedef struct BST{
    bst_compare_t compare;
    BST_node *root;
}BST;

struct BST* getNewinitTree(bst_compare_t compare);
struct BST_node* insertToNode(BST_node* node, char* key, void* value, bst_compare_t compare);
void insertElem(struct BST* tree, char* key, void* value, bst_compare_t compare);
void* bst_lookUpValue_elem(BST_node* node, char* key, bst_compare_t compare);
void* bst_lookUpValue(struct BST* tree, char* key,bst_compare_t compare);
int stringcmp(char* s1, char* s2);
void printBST_node(BST_node* node);
void printBST(struct BST* tree);
void bst_destroy_elem(struct BST_node * node);
void bst_destroy(struct BST* tree);

/////////////////////////////////////////////////////////////////////////////////////////
// structures for the table mapping line_tokens to instructions
/////////////////////////////////////////////////////////////////////////////////////////

struct table{
    char* opcode;
    void (*func)(LINE_TOKEN*, INSTRUCTION*);
};

struct branchelem{
    char* opcode;
    void (*br)(LINE_TOKEN*, INSTRUCTION*, struct Linkedlist*, u32 currAddress);
};

struct sdtelem{
    char* opcode;
    void (*sdt)(LINE_TOKEN*, INSTRUCTION*, u32 currAddress, u32 numOfInstructions);
};

/////////////////////////////////////////////////////////////////////////////////////////
// functions to assemble the instructions
/////////////////////////////////////////////////////////////////////////////////////////

void assembleAdd(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleSub(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleRsb(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleAnd(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleEor(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleOrr(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleMov(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleTst(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleTeq(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleCmp(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleMulHelper(LINE_TOKEN *line_token, INSTRUCTION *instr);
void assembleMul(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleMla(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleLdr(LINE_TOKEN *line_token, INSTRUCTION *instr, u32 currAddress, u32 numOfInstructions);
void assembleStr(LINE_TOKEN *line_token, INSTRUCTION *instr, u32 currAddress, u32 numOfInstructions);
void assembleBeq(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleBne(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleBge(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleBlt(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleBgt(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleBle(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleB(LINE_TOKEN* line_token, INSTRUCTION* instr, struct Linkedlist *symboltable, u32 currAddress);
void assembleLsl(LINE_TOKEN* line_token, INSTRUCTION* instr);
void assembleAndeq(LINE_TOKEN* line_token, INSTRUCTION* instr);

/////////////////////////////////////////////////////////////////////////////////////////
// functions to map string opcodes to function pointers to assembler the instructions
/////////////////////////////////////////////////////////////////////////////////////////

typedef void(*assemblefunction)(LINE_TOKEN*, INSTRUCTION*);
typedef void(*assembleBranch)(LINE_TOKEN*, INSTRUCTION*, struct Linkedlist* symbolTable, u32 currentAddress);
typedef void(*assembleSdt)(LINE_TOKEN*, INSTRUCTION*, u32 currentAddress, u32 numOfInstructions);
assemblefunction lookUpfunction(char* instr);
assembleBranch lookUpBranch(char* instr);
assembleSdt lookUpSdt(char* instr);
u32 assembleInstructions(INSTRUCTION* instr);

/////////////////////////////////////////////////////////////////////////////////////////
// functions implementing the two pass assembly
/////////////////////////////////////////////////////////////////////////////////////////
u32 firstpass(LINE_TOKEN **line_tokens, struct Linkedlist **symbolTable, int numOfLines);
u32 secondpass(LINE_TOKEN *line_tokens[], u32 *Memory, struct Linkedlist **symbolTable, int numOfLines,
               u32 numOfInstructions);

/////////////////////////////////////////////////////////////////////////////////////////
// Helper functions for the assembling functions
/////////////////////////////////////////////////////////////////////////////////////////
u32 transformnum(int num);
void lsltoMov(LINE_TOKEN* line_token);
u32 parseRegister(char* operand);
u32 parseExpression(char* operand);
u32 shifting(LINE_TOKEN *line_token, int pos);

/////////////////////////////////////////////////////////////////////////////////////////
// Structure for parsing operands inside an address
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct{
    int Num_Opr_in_bracket;
    int Num_Opr_not_in_bracket;
    //max of 2 operands in bracket
    char *Opr_in_Bracket[2];
    //max of 1 operands not in bracket
    char *Opr_not_in_bracket[1];
}ADDRESS;

/////////////////////////////////////////////////////////////////////////////////////////
// Functions for reading source files and writing binary files
/////////////////////////////////////////////////////////////////////////////////////////
void binaryFileWriter(u32 instr[], char *path, u32 numOfInstructions);
u32 sourceFileReader(char* lines[], char* filename);

/////////////////////////////////////////////////////////////////////////////////////////
// Array for holding the memory read from the single data transfer load function,
// where the extra number is placed after the instructions after assembling
/////////////////////////////////////////////////////////////////////////////////////////
u32 Memory[100];
u32 memoryPos;
#endif //ARM11_06_ASSEMBLE_H

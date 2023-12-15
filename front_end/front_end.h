#ifndef FRONT
#define FRONT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const int size = 50;
const int free_cell = -1;

struct Name
{
    int type;
    int name_code;
    char* value;
};

struct TreeNode
{
    TreeNode* left;
    TreeNode* right;
    float value;
    int type;
};

struct Tree
{
    TreeNode* root;
};

enum OPERATORS
{
    OP_DIV = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_ADD = 4,
    OP_EXP = 5,

    EQUAL = 11,
    LIMIT = 12,
    SLASH = 13,
    AND = 14,
    MORE = 16,
    LESS = 17,
};

enum FUNCTIONS
{
    SIN = 6,
    COS = 7,
    LN = 8,
    TAN = 9,
    COT = 10,
};

enum KEY_WORDS
{
    IF = 1,
    WHILE = 2,
};

enum TYPES
{
    KWR = 1,
    VAR = 2,
    FUNC = 3,
    OP = 4,
    NUM = 5,
    FUNC_HEAD = 15,
};

void TreeDel (TreeNode* node);
void DumpTreeNode (TreeNode* node, FILE* file, Name* name_cell);
char GetOpChar (int command);
char* GetFuncName (int func, Name* name_cell);
TreeNode* NewNode (enum TYPES node_type, int node_value, TreeNode* left, TreeNode* right);
long FileSize1 (FILE* file);
char* ReadText1 (long file_size, FILE* file);
char* TreeCtor1 (Tree* tree, FILE* file);
void TreeDump (TreeNode* node, Name* name_cell);
int GetOpCode (char op);
int SearchFreeCell (Name* name_cell);
int SearchFuncName (char* name, Name* name_cell);
int SearchVarName (char* name, Name* name_cell);

#endif

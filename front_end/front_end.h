#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int size = 50;
const int free = -1;

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
    SEMICOLON = 12,
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
};

int SearchFreeCell (Name* name_cell);
int SearchFuncName (char* name, Name* name_cell);
int SearchKeyWordName (char* name, Name* name_cell);

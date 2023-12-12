#ifndef DESENT
#define DESENT

#include "language_decent.h"
#include "front_end.h"

struct Position
{
    const char* str;
    int position;
};

TreeNode* GetIf_WHILE (Position* data, Name* name_cell);
TreeNode* GetOp (Position* data, Name* name_cell);
TreeNode* GetExp (Position* data, Name* name_cell);
TreeNode* GetG (const char* str, Position* data, Name* name_cell);
TreeNode* GetN (Position* data);
TreeNode* GetT (Position* data, Name* name_cell);
TreeNode* GetE (Position* data, Name* name_cell);
TreeNode* GetP (Position* data, Name* name_cell);
TreeNode* GetId (Position* data, Name* name_cell);
TreeNode* GetA (Position* data, Name* name_cell);
TreeNode* GetLim (Position* data, Name* name_cell);

void SkipSpaces (Position* data);

#endif

#include "../back_end/back_end.h"
#include "../front_end/front_end.h"
#include "../back_end/stack/stack_cd.h"

int num_of_while = 0;
int num_of_if = 0;

void ConvertToAssembler (TreeNode* node, FILE* file, Name* name_cell)
{
    stack while_num = {};
    StackCtor (&while_num, num_size);
    stack if_num = {};
    StackCtor (&if_num, num_size);

    if (node->type == KWR && node->value == WHILE) // изменить так как while может не соответствовать условию
    {
        StackPush (&while_num, (float)num_of_while);

        ConvertToAssembler (node->left, file, name_cell);
        if (node->left->value == MORE)
        {
            float tmp_num = 0;
            StackPop (&while_num, &tmp_num);
            fprintf (file, "ja :while_%d\n", (int)tmp_num);
            fprintf (file, "jbe :then_while_%d\n", (int)tmp_num);
            StackPush (&while_num, tmp_num);

        }
        else if (node->left->value == LESS)
        {
            float tmp_num = 0;
            StackPop (&while_num, &tmp_num);
            fprintf (file, "jb :while_%d\n", (int)tmp_num);
            fprintf (file, "jae :then_while_%d\n", (int)tmp_num);
            StackPush (&while_num, tmp_num);
        }

        fprintf (file, ":while_%d\n", num_of_while);
        num_of_while++;

        if (node->right != NULL)
            ConvertToAssembler (node->right, file, name_cell);

        if (node->left->type == OP)
        {
            ConvertToAssembler (node->left, file, name_cell);
            if (node->left->value == MORE)
            {
                float tmp_num = 0;
                StackPop (&while_num, &tmp_num);
                fprintf (file, "ja :while_%d\n", (int)tmp_num);
                fprintf (file, "jbe :then_while_%d\n", (int)tmp_num);
            }
            else if (node->left->value == LESS)
            {
                float tmp_num = 0;
                StackPop (&while_num, &tmp_num);
                fprintf (file, "jb :while_%d\n", (int)tmp_num);
                fprintf (file, "jbe :then_while_%d\n", (int)tmp_num);
            }
        }
        fprintf (file, ":then_while_%d\n", num_of_while - 1);
    }
    else if (node->type == KWR && node->value == IF)
    {
        StackPush (&if_num, (float)num_of_if);
        num_of_if++;

        if (node->left->type == OP)
        {
            ConvertToAssembler (node->left, file, name_cell);
            if (node->left->value == MORE)
            {
                float tmp_num = 0;
                StackPop (&if_num, &tmp_num);
                fprintf (file, "ja :if_%d\n", (int)tmp_num);
                fprintf (file, "jbe :then_if_%d\n", (int)tmp_num);
            }
            else if (node->left->value == LESS)
            {
                float tmp_num = 0;
                StackPop (&if_num, &tmp_num);
                fprintf (file, "jb :if_%d\n", (int)tmp_num);
                fprintf (file, "jae :then_if_%d\n", (int)tmp_num);
            }
        }
        fprintf (file, ":if_%d\n", num_of_if - 1);
        if (node->right != NULL)
            ConvertToAssembler (node->right, file, name_cell);
        fprintf (file, ":then_if_%d\n", num_of_if - 1);
    }

    if (node->type != KWR || (node->value != WHILE && node->value != IF))
    {
        if (node->left != NULL)
            ConvertToAssembler (node->left, file, name_cell);
        if (node->right != NULL && (node->type != OP || node->value != EQUAL))
            ConvertToAssembler (node->right, file, name_cell);
    }

    if (node->type == NUM)
    {
        fprintf (file, "push %f\n", node->value);
    }
    else if (node->type == OP)
    {
        if (node->value == OP_ADD)
            fprintf (file, "add\n");
        else if (node->value == OP_SUB)
            fprintf (file, "sub\n");
        else if (node->value == OP_MUL)
            fprintf (file, "mul\n");
        else if (node->value == OP_DIV)
            fprintf (file, "div\n");
        else if (node->value == EQUAL)
            if (node->right->type == VAR)
                fprintf (file, "pop [%d]\n", (int)node->right->value);
    }
    else if (node->type == VAR)
    {
        fprintf (file, "push [%d]\n", (int)node->value);
    }
}

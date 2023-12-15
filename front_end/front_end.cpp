#include "../front_end/front_end.h"

TreeNode* NewNode (enum TYPES node_type, int node_value, TreeNode* left, TreeNode* right)
{
    TreeNode* node = (TreeNode*)calloc (1, sizeof (TreeNode));
    node->type = node_type;
    node->value = node_value;
    node->left = left;
    node->right = right;
    return node;
}

int GetOpCode (char op)
{
    if (op == '=')
        return EQUAL;
    else if (op == '<')
        return MORE;
    else if (op == '>')
        return LESS;
}

void TreeDump (TreeNode* node, Name* name_cell)
{
    FILE* file = NULL;
    file = fopen ("front_end/front_end.dot", "w");

    fprintf (file, "digraph G { \n"
                    "node [shape = record];\n"
                    " %o ", node);

    DumpTreeNode (node, file, name_cell);
    fprintf (file, "}\n");
    fclose (file);

    system ("dot -T png front_end/front_end.dot -o front_end/front_end.png");
}

char GetOpChar (int command)
{
    switch (command)
    {
        case OP_MUL:
            return '*';
        case OP_DIV:
            return '/';
        case OP_ADD:
            return '+';
        case OP_SUB:
            return '-';
        case OP_EXP:
            return '^';
        case EQUAL:
            return '=';
        case LIMIT:
            return '`';
        case SLASH:
            return '/'; // подумать по этому поводу
                        // сливается с делением
        case AND:
            return '&';
        case MORE:
            return '<';
        case LESS:
            return '>';
    };

    return 0;
}

char* GetFuncName (int func, Name* name_cell)
{
    return name_cell[func].value;
    // name code <=> cell_num
}

void DumpTreeNode (TreeNode* node, FILE* file, Name* name_cell)
{
    if (node->type == NUM)
    {
        fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = cornflowerblue "
                       " label = \" %f \"];\n",
                       node,
                       (float)node->value);
    }
    else if (node->type == FUNC)
    {
        fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = aqua "
                       " label = \" %s \"];\n",
                       node,
                       GetFuncName ((int)node->value, name_cell));
    }
    else if (node->type == FUNC_HEAD)
    {
        fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = aqua "
                       " label = \" %s* \"];\n",
                       node,
                       GetFuncName ((int)node->value, name_cell));
    }
    else if (node->type == OP)
    {
        if (node->value == LIMIT || node->value == SLASH)
            fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = aquamarine "
                           " label = \" %c \"];\n",
                           node,
                           GetOpChar ((int)node->value));
        else if (node->value == AND)

            fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = green "
                           " label = \" %c \"];\n",
                           node,
                           GetOpChar ((int)node->value));
        else
            fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = aqua "
                           " label = \" %c \"];\n",
                           node,
                           GetOpChar ((int)node->value));
    }
    else if (node->type == VAR)
    {
        fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = lightblue2 "
                       " label = \" %s \"];\n",
                       node,
                       name_cell[(int)node->value].value);
    }
    else if (node->type == KWR)
    {
        if (node->value == IF)
            fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = brown1 "
                           " label = \" if \"];\n",
                           node);
        else if (node->value == WHILE)
            fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = brown1 "
                           " label = \" while \"];\n",
                           node);
    }

    if (node->type != FUNC)
    {
        if (node->left != NULL)
            fprintf (file, "%o -> %o", node, node->left);
        else
            return;
    }

    if ((node->type != OP || node->value != LIMIT || node->right != NULL) &&
        (node->type != OP || node->value != SLASH || node->right != NULL) &&
        (node->type != FUNC_HEAD || node->right != NULL))
    {
        if (node->right != NULL)
            fprintf (file, "\n %o -> %o \n", node, node->right);
        else
            return;
    }

    if (node->type != FUNC)
        DumpTreeNode (node->left, file, name_cell);
    if (node->right != NULL)
        DumpTreeNode (node->right, file, name_cell);
}

int SearchVarName (char* name, Name* name_cell)
{
    for (int i = 0; i < size; i++)
    {
        if (name_cell[i].type == VAR)
        {
            if (strcmp (name, name_cell[i].value) == 0)
            {
                return name_cell[i].name_code;
            }
        }
    }
    int cell = SearchFreeCell (name_cell);
    name_cell[cell].type = VAR;
    name_cell[cell].value = strdup (name);
    name_cell[cell].name_code = cell;
    return cell;
}

int SearchFuncName (char* name, Name* name_cell)
{
    for (int i = 0; i < size; i++)
    {
        if (name_cell[i].type == FUNC)
        {
            if (strcmp (name, name_cell[i].value) == 0)
            {
                return name_cell[i].name_code;
            }
        }
    }
    int cell = SearchFreeCell (name_cell);
    name_cell[cell].type = FUNC;
    name_cell[cell].value = strdup (name);
    name_cell[cell].name_code = cell;
    return cell;
}

int SearchFreeCell (Name* name_cell)
{
    for (int i = 0; i < size; i++)
    {
        if (name_cell[i].type == free_cell)
        {
            return i;
        }
    }
    //name_cell = (Name*)realloc (name_cell, size * 2); // передать новое значение size
    //return size; // опять же это еще старое значение, нужно поменять
}

char* TreeCtor1 (Tree* tree, FILE* file)
{
    tree->root = (TreeNode*)calloc (1, sizeof (TreeNode));
    tree->root->value = 0;
    tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->type = 0;

    long file_size = FileSize1 (file);
    char* tmp_buf = ReadText1 (file_size, file);
    fclose (file);
    return tmp_buf;
}

char* ReadText1 (long file_size, FILE* file)
{
    char* buf = (char*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

long FileSize1 (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long file_size = ftell (file);
    fseek (file, 0, SEEK_SET);
    return file_size;
}

void TreeDel (TreeNode* node)
{
    if (node != NULL)
    {
        TreeDel (node->left);
        TreeDel (node->right);
        free (node);
    }
}

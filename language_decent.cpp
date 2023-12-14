#include "language_decent.h"
#include "front_end.h"

void SkipSpaces (Position* data)
{
    while (data->str[data->position] == ' ' || data->str[data->position] == '\n')
    {
        data->position++;
    }
}

TreeNode* GetP (Position* data, Name* name_cell)
{
    SkipSpaces (data);
    if (data->str[data->position] == '(')
    {
        TreeNode* val = 0;
        data->position++;
        val = GetE (data, name_cell);
        data->position++;
        return val;
    }

    if (isalpha (data->str[data->position]) != 0)
    {
        return GetId (data, name_cell);
    }
    else
    {
        return GetN (data);
    }
}

TreeNode* GetT (Position* data, Name* name_cell)
{
    TreeNode* val = GetExp (data, name_cell);
    SkipSpaces (data);
    while (data->str[data->position] == '*' || data->str[data->position] == '/') // if
    {
        int op = data->str[data->position];
        data->position++;
        TreeNode* val2 = GetExp (data, name_cell);
        switch (op)
        {
            case '*':
            {
                val = NewNode (OP, OP_MUL, val, val2);
                break;
            }
            case '/':
            {
                val = NewNode (OP, OP_DIV, val, val2);
                break;
            }
        }
    }
    return val;
}

TreeNode* GetE (Position* data, Name* name_cell)
{
    TreeNode* val = GetT (data, name_cell);
    SkipSpaces (data);
    while (data->str[data->position] == '+' || data->str[data->position] == '-')
    {
        int op = data->str[data->position];
        data->position++;
        TreeNode* val2 = GetT (data, name_cell);
        switch (op)
        {
            case '+':
            {
                val = NewNode (OP, OP_ADD, val, val2);
                break;
            }
            case '-':
            {
                val = NewNode (OP, OP_SUB, val, val2);
                break;
            }
        }
    }
    return val;
}

TreeNode* GetG (const char* str, Position* data, Name* name_cell)
{
    data->str = str;
    data->position = 0;
    TreeNode* val = GetFunc (data, name_cell);
    return val;
}

TreeNode* GetLim (Position* data, Name* name_cell)
{
    char arg[20] = "";
    int counter = 0;
    SkipSpaces (data);
    int old_position = data->position;
    while (isalpha (data->str[data->position]) != 0)
    {
        sprintf (arg + counter, "%c", data->str[data->position]);
        data->position++;
        counter++;
    }
    if (strcmp (arg, "if") == 0 || strcmp (arg, "while") == 0)
    {
        data->position = old_position;
        TreeNode* val = GetCondOp (data, name_cell);
        data->position++;
        SkipSpaces (data);
        if (data->str[data->position] != '.' && data->str[data->position] != '/')
        {
            TreeNode* val2 = GetLim (data, name_cell);
            return NewNode (OP, SLASH, val, val2);
        }
        data->position++;
        return NewNode (OP, SLASH, val, NULL);
    }
    data->position = old_position;
    TreeNode* val = GetA (data, name_cell); // +
    SkipSpaces (data);
    int pre_position = data->position;
    data->position++;
    SkipSpaces (data);
    if (data->str[data->position] != '/' && data->str[data->position] != '.')
    { // все лежит в функциях => не может закончится \0
        TreeNode* val2 = GetLim (data, name_cell);
        return NewNode (OP, LIMIT, val, val2);
    }
    else if (data->str[data->position] == '/' && data->str[pre_position] == '`')
    {
        return NewNode (OP, LIMIT, val, NULL);
    }
    else if (data->str[data->position] == '/')
    {
        return NewNode (OP, SLASH, val, NULL);
    }
    return NewNode (OP, LIMIT, val, NULL);
}

TreeNode* GetDifferentCond (Position* data, Name* name_cell)
{
    TreeNode* val = GetA (data, name_cell);
    SkipSpaces (data);
    if (data->str[data->position] == '&')
    {
        data->position += 2;
        TreeNode* val2 = GetDifferentCond (data, name_cell);
        return NewNode (OP, AND, val, val2);
    }
    return val;
}

TreeNode* GetA (Position* data, Name* name_cell)
{
    TreeNode* val = GetE (data, name_cell);
    SkipSpaces (data);
    // изменить и не давать ставить знаки сравнения в теле циклов и условных операторов
    if (data->str[data->position] == '=' ||
        data->str[data->position] == '>' ||
        data->str[data->position] == '<') // добавить больше/меньше
    {
        char operation = data->str[data->position];
        data->position++;
        TreeNode* val2 = GetId (data, name_cell);
        return NewNode (OP, GetOpCode (operation), val, val2);
    }
    return val;
}

TreeNode* GetFunc (Position* data, Name* name_cell)
{
    char arg[20] = "";
    int counter = 0;
    SkipSpaces (data);
    int old_position = data->position;
    while (isalpha (data->str[data->position]) != 0)
    {
        sprintf (arg + counter, "%c", data->str[data->position]);
        data->position++;
        counter++;
    }
    SkipSpaces (data);
    if (data->str[data->position] == '(') // если делать функцию с переменными - исправить
    {
        data->position++;
        SkipSpaces (data);
        if (data->str[data->position] == ')')
            data->position++;
        SkipSpaces (data);
        TreeNode* val = GetLim (data, name_cell);
        printf ("dot = %c\n", data->str[data->position]);
        data->position++;
        SkipSpaces (data);
        if (data->str[data->position] == '\0')
            return NewNode (FUNC_HEAD, SearchFuncName (arg, name_cell), val, NULL); // прописать случай когда ненкций много
        else
            return NewNode (FUNC_HEAD, SearchFuncName (arg, name_cell), val, GetFunc (data, name_cell));
    }
    data->position = old_position;
    return GetLim (data, name_cell);
}

TreeNode* GetCondOp (Position* data, Name* name_cell)
{
    char arg[20] = "";
    int counter = 0;
    SkipSpaces (data);
    int old_position = data->position; //??
    while (isalpha (data->str[data->position]) != 0)
    {
        sprintf (arg + counter, "%c", data->str[data->position]);
        data->position++;
        counter++;
    }
    if (strcmp (arg, "if") == 0)
    {
        TreeNode* val = GetDifferentCond (data, name_cell);
        TreeNode* val2 = GetOp (data, name_cell);
        return NewNode (KWR, IF, val, val2);
    }
    else if (strcmp (arg, "while") == 0)
    {
        TreeNode* val = GetDifferentCond (data, name_cell);
        TreeNode* val2 = GetOp (data, name_cell);
        return NewNode (KWR, WHILE, val, val2);
    }
    return 0;
}

TreeNode* GetOp (Position* data, Name* name_cell)
{
    SkipSpaces (data);
    if (data->str[data->position] == '\\')
    {
        printf ("OPPPPP\n");
        data->position++;
        TreeNode* val = GetLim (data, name_cell);
        return val;
    }
    return 0;
}

TreeNode* GetId (Position* data, Name* names)
{
    char arg[20] = "";
    int counter = 0;
    SkipSpaces (data);
    while (isalpha (data->str[data->position]) != 0)
    {
        sprintf (arg + counter, "%c", data->str[data->position]);
        data->position++;
        counter++;
    }
    printf ("%s\n", arg);
    SkipSpaces (data);
    int func_name_code = SearchFuncName (arg, names);
    printf ("func_code = %d\n", func_name_code);
    if (data->str[data->position] == '(')
    {
        TreeNode* val = NULL;
        data->position++;
        val = GetE (data, names);
        data->position++;
        return NewNode (FUNC, func_name_code, NULL, val);
    }

    return NewNode (VAR, SearchVarName (arg, names), NULL, NULL);
}

TreeNode* GetExp (Position* data, Name* name_cell)
{
    TreeNode* val = GetP (data, name_cell);
    SkipSpaces (data);
    if (data->str[data->position] == '^')
    {
        data->position++;
        TreeNode* val2 = GetP (data, name_cell);
        val = NewNode (OP, OP_EXP, val, val2);
    }
    return val;
}

TreeNode* GetN (Position* data)
{
    int val = 0;
    int old_p = data->position;
    SkipSpaces (data);
    while ('0' <= data->str[data->position] && data->str[data->position] <= '9')
    {
        val = val * 10 + data->str[data->position] - '0';
        data->position++;
    }
    return NewNode (NUM, val, NULL, NULL);
}

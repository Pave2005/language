#include "../front_end/language_decent.h"
#include "../front_end/front_end.h"

void SkipSpaces (Position* data)
{
    while (data->str[data->position] == ' ' || data->str[data->position] == '\n')
    {
        data->position++;
    }
}

TreeNode* GetP (Position* data)
{
    SkipSpaces (data);
    if (data->str[data->position] == '(')
    {
        TreeNode* val = 0;
        data->position++;
        val = GetE (data);
        data->position++;
        return val;
    }
    SkipSpaces (data);
    if (isalpha (data->str[data->position]) != 0)
    {
        return GetId (data);
    }
    else
    {
        return GetN (data);
    }
}

TreeNode* GetT (Position* data)
{
    TreeNode* val = GetExp (data);
    while (data->str[data->position] == '*' || data->str[data->position] == '/') // if
    {
        int op = data->str[data->position];
        data->position++;
        TreeNode* val2 = GetExp (data);
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

TreeNode* GetE (Position* data)
{
    TreeNode* val = GetT (data);
    SkipSpaces (data);
    while (data->str[data->position] == '+' || data->str[data->position] == '-')
    {
        int op = data->str[data->position];
        data->position++;
        TreeNode* val2 = GetT (data);
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

TreeNode* GetG (const char* str, Position* data)
{
    data->str = str;
    data->position = 0;
    TreeNode* val = GetIf_WHILE (data);
    return val;
}

TreeNode* GetA (Position* data)
{
    TreeNode* val = GetE (data);
    SkipSpaces (data);
    if (data->str[data->position] == '=')
    {
        data->position++;
        TreeNode* val2 = GetId (data);
        return NewNode (OP, EQUAL, val, val2);
    }
    return val;
}

TreeNode* GetIf_WHILE (Position* data) // разделить
{
    char arg[20] = "";
    int counter = 0;
    SkipSpaces (data);
    while (isalpha (data->str[data->position]) != 0)
    {
        sprintf (arg + counter, "%c", data->str[data->position]); // заменить на обычный массив
        data->position++;
        counter++;
    }
    if (strcmp (arg, "if") == 0)
    {
        TreeNode* val = GetA (data); // должен сдвинуть позицию до {
        data->position++;
        TreeNode* val2 = GetOp (data);
        return NewNode (KWR, IF, val, val2);
    }
    else if (strcmp (arg, "while") == 0)
    {
        TreeNode* val = GetA (data);
        data->position++;
        TreeNode* val2 = GetOp (data);
        return NewNode (KWR, WHILE, val, val2);
    }
    return GetA (data);
}

TreeNode* GetOp (Position* data)
{
    SkipSpaces (data);
    if (data->str[data->position] == '{')
    {
        TreeNode* val = GetIf_WHILE (data); // предположительно сдвигает ползунок
        SkipSpaces (data);
        if (data->str[data->position] != '}')
        {
            TreeNode* val2 = GetOp (data);
            return NewNode (OP, SEMICOLON, val, val2);
        }
        return NewNode (OP, SEMICOLON, val, NULL);
    }
    return 0; // возвращать код ошибки
}

TreeNode* GetN (Position* data)
{
    int val = 0;
    SkipSpaces (data);
    int old_p = data->position; // для вывода ошибки
    while ('0' <= data->str[data->position] && data->str[data->position] <= '9')
    {
        val = val * 10 + data->str[data->position] - '0';
        data->position++;
    }
    return NewNode (NUM, val, NULL, NULL);
}

TreeNode* GetId (Position* data, Name* names)
{
    char arg[20] = "";
    int counter = 0;
    SkipSpaces (data);
    while (isalpha (data->str[data->position]) != 0)
    {
        sprintf (arg + counter, "%c", data->str[data->position]); // заменить на обычный массив
        data->position++;
        counter++;
    }
    // поправить
    int func_name_code = SearchFuncName (arg, names);
    if (func_name_code != 0 && data->str[data->position] == '(')
    {
        TreeNode* val = NULL;
        data->position++;
        val = GetE (data);
        data->position++;
        return NewNode (FUNC, func_name_code, NULL, val);
    }

    int kwr_name_code = SearchKeyWordName (arg, names); // If
    if (kwr_name_code != 0)
    {
        return kwr_name_code;
    }



}

TreeNode* GetExp (Position* data)
{
    TreeNode* val = GetP (data);
    SkipSpaces (data);
    if (data->str[data->position] == '^')
    {
        data->position++;
        TreeNode* val2 = GetP (data);
        val = NewNode (OP, OP_EXP, val, val2);
    }
    return val;
}

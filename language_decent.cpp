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
    TreeNode* val = GetLim (data, name_cell);
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
        sprintf (arg + counter, "%c", data->str[data->position]); // заменить на обычный массив
        data->position++;
        counter++;
    } // посмотрели что за элемент
    if (strcmp (arg, "if") == 0 || strcmp (arg, "while") == 0)
    {
        data->position = old_position;
        TreeNode* val = GetIf_WHILE (data, name_cell); //++
        //TreeDump (val, name_cell);
        printf ("<%d>\n", data->position);
        SkipSpaces (data);
        if (data->str[data->position] != '\0' && data->str[data->position] != '/') // не факт что эти символы вообще там лежат, 0 должен
        {
            TreeNode* val2 = GetLim (data, name_cell);
            return NewNode (OP, SLASH, val, val2);
        }
        return NewNode (OP, SLASH, val, NULL); // не splash возвращаем
    }
    data->position = old_position;
    TreeNode* val = GetA (data, name_cell); // вернула // +++
    //TreeDump (val, name_cell); // А корректно возвращает
    SkipSpaces (data);
    int pre_position = data->position;
    data->position++; // если символ ` сдивинули, если нет - ошибка
    SkipSpaces (data);
    // в этой части ошибка
    if (data->str[data->position] != '\0' && data->str[data->position] != '/') // проверить в этой ли ячейке будет лежать символ
    {
        printf ("LIM/2\n");
        TreeNode* val2 = GetLim (data, name_cell); // +
        return NewNode (OP, LIMIT, val, val2);
    }
    else if (data->str[data->position] == '/' && data->str[pre_position] == '`')
    {
        printf ("LIM/NULL\n");
        //TreeDump (NewNode (OP, LIMIT, val, NULL), name_cell);
        return NewNode (OP, LIMIT, val, NULL); // этот не правильно возвращает
    }
    else if (data->str[data->position] == '/')
    {
        printf ("SLASH\n");
        return NewNode (OP, SLASH, val, NULL);
    }
    printf ("OK\n ");
    return NewNode (OP, LIMIT, val, NULL);
}

TreeNode* GetA (Position* data, Name* name_cell)
{
    TreeNode* val = GetE (data, name_cell);
    //printf ("%c\n", data->str[data->position]);
    SkipSpaces (data);
    if (data->str[data->position] == '=')
    {
        data->position++;
        TreeNode* val2 = GetId (data, name_cell); // возможно изменить на getP
        return NewNode (OP, EQUAL, val, val2);
    }
    return val;
}

TreeNode* GetIf_WHILE (Position* data, Name* name_cell) // разделить
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
    if (strcmp (arg, "if") == 0)
    {
        TreeNode* val = GetA (data, name_cell); // без дополнительных ограничителей в конце
        TreeNode* val2 = GetOp (data, name_cell);
        //TreeDump (val2, name_cell);
        return NewNode (KWR, IF, val, val2);
    }
    else if (strcmp (arg, "while") == 0)
    {
        TreeNode* val = GetA (data, name_cell);
        TreeNode* val2 = GetOp (data, name_cell);
        return NewNode (KWR, WHILE, val, val2);
    }
    //data->position = old_position; // ??
    //TreeNode* val = GetA (data, name_cell); // ??
    //return val;
    return 0;
}

TreeNode* GetOp (Position* data, Name* name_cell) ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    SkipSpaces (data);
    if (data->str[data->position] == '\\') // возможно с этим вопрос
    {
        printf ("OPPPPP\n");
        data->position++;
        TreeNode* val = GetLim (data, name_cell); // предположительно сдвигает ползунок
        //TreeDump (val, name_cell);
        //data->position++; // возможно нужно без нее
        return val;
    }
    return 0; // возвращать код ошибки
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
    if (func_name_code != 0 && data->str[data->position] == '(')
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

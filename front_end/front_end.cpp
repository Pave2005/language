#include "../front_end/front_end.h"

int SearchKeyWordName (char* name, Name* name_cell)
{
    for (int i = 0; i < size; i++)
    {
        if (name_cell[i].type == KWR)
        {
            if (strcmp (name, name_cell[i].value) == 0)
            {
                return name_cell[i].name_code;
            }
        }
    }
    int cell = SearchFreeCell (name_cell);
    name_cell[cell].type = KWR;
    name_cell[cell].value = strdup (name);

    if (strcmp ("if", name) == 0)
    {
        name_cell[cell].name_code = IF;
        return IF;
    }
    else if (strcmp ("while", name) == 0)
    {
        name_cell[cell].name_code = WHILE;
        return WHILE;
    }

    name_cell[cell].type = free;
    name_cell[cell].value = NULL;
    return 0; // если это не функция
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
    if (strcmp ("sin", name) == 0)
    {
        name_cell[cell].name_code = SIN;
        return SIN;
    }
    else if (strcmp ("cos", name) == 0)
    {
        name_cell[cell].name_code = COS;
        return COS;
    }
    else if (strcmp ("tg", name) == 0)
    {
        name_cell[cell].name_code = TAN;
        return TAN;
    }
    else if (strcmp ("ctg", name) == 0)
    {
        name_cell[cell].name_code = COT;
        return COT;
    }
    else if (strcmp ("ln", name) == 0)
    {
        name_cell[cell].name_code = LN; // сделать функцию и отдельно рассматривать арифметические ключевые слова и слова языка
        return LN;
    }

    name_cell[cell].type = free;
    name_cell[cell].value = NULL;
    return 0; // если это не функция
}

int SearchFreeCell (Name* name_cell)
{
    for (int i = 0; i < size; i++)
    {
        if (name_cell[i].type == free)
        {
            return i;
        }
    }
    name_cell = (Name*)realloc (name_cell, size * 2); // передать новое значение size
    return size; // опять же это еще старое значение, нужно поменять
}

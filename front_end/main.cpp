#include <stdio.h>
#include <stdlib.h>
#include "../front_end/front_end.h"
#include "../front_end/language_decent.h"
#include "../back_end/back_end.h"

int main ()
{
    Name* names = (Name*)calloc (size, sizeof (Name));
    for (int i = 0; i < size; i++)
        names[i].type = free_cell;
    Tree tree = {};
    FILE* file = fopen ("front_end/file.txt", "r");

    Position data = {};

    char* data_buf = TreeCtor1 (&tree, file);
    tree.root = GetG (data_buf, &data, names); // построимли дерево

    // front_end
    FILE* file_ = fopen ("back_end/command.txt", "w");
    ConvertToAssembler (tree.root, file_, names);
    TreeDump (tree.root, names);
    TreeDel (tree.root);
    fclose (file_);
}

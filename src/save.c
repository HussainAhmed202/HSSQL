#include <stdio.h>

typedef struct table
{
    int num_of_col;
    char name[50];
} table;

int save_object(char *file_name, table t)
{
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        return 1;
    }

    size_t written = fwrite(&t, sizeof(t), 1, file);

    if (written == 1)
    {
        fclose(file);
        return 2;
    }
    else
    {
        fclose(file);
        return 3;
    }
}

int main()
{
    table t1 = {2, "users"};
    size_t written = save_object("data/table.dat", t1);
    if (written == 2)
    {
        printf("Struct written to file successfully.\n");
    }
    else if (written == 3)
    {
        printf("Error writing struct to file.\n");
    }
    else if (written == 1)
    {
        printf("Error opening file\n");
    }
    else
    {
        printf("Something issue happended\n");
    }

    return 0;
}
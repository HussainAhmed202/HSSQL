#include <stdio.h>
#include <string.h>
#define MAX_COLUMNS 10

typedef struct
{
    char col_name[100];
    char col_type[50];
} Column;

int main()
{
    int token_count = 10;
    // In main file, this array will be populated by the tokenizer
    char token[10][10] = {
        "CREATE",
        "TABLE",
        "users",
        "(",
        "id",
        "INT",
        ",",
        "name",
        "TEXT",
        ")",
    };

    int num_columns = 0;
    char table_name[35] = {0};
    Column column[MAX_COLUMNS] = {0};

    int j = 0; // used to track which column array index to populate;
    int k = 0; // for struct field tracking; k=0 populate col_name; k=1 populate col_type

    if (strcmp(token[0], "CREATE") != 0)
    {
        printf("Syntax error: Expected CREATE statement.\n");
        return -1;
    }

    if (strcmp(token[1], "TABLE") != 0)
    {
        printf("Syntax error: Expected TABLE after CREATE.");
        return -1;
    }
    strcpy(table_name, token[2]);

    if (strcmp(token[3], "(") != 0)
    {
        printf("Syntax error : Expected '(' after table name.");
        return -1;
    }

    // looping inside the (...) block. Pick each column name and its type that is being defined in the CREATE statement
    for (size_t i = 4; i < (size_t)token_count; i++)
    // starting after opening bracket - [0]CREATE [1]TABLE [2]<table-name> [3](
    {
        if (strcmp(token[i], ")") == 0)
        {
            // printf("Loop end condition encountered. Break");
            break;
        }

        if (k == 0) // populate the first struct member
        {
            // printf("column_name -> %s\n", token[i]);
            strcpy(column[j].col_name, token[i]);
            k++; // first struct member populated. Now the second will be populated
        }
        else if (k == 1) // populate the second struct member
        {
            // printf("column_type -> %s\n", token[i]);
            strcpy(column[j].col_type, token[i]);
            k++; // second struct member populated. Now comma expected. Move to next struct in the array
        }
        else // comma encountered
        {
            k = 0;
            j++;
            num_columns++; // a column was completely defined [name type]
        }
    }

    // at k =2 comma is encountered. This is reset. For the last condition no comma encountered
    if (k == 2)
    {
        num_columns++; // a column was completely defined prior to closure [name type])
    }

    printf("Total number of columns %d\n", num_columns);

    for (size_t i = 0; i < (size_t)num_columns; i++)
    {
        printf("Column[%zu].col_name -> %s\n", i, column[i].col_name);
        printf("Column[%zu].col_type -> %s\n", i, column[i].col_type);
    }

    return 0;
}
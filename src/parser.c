#include <stdio.h>
#include <string.h>
#include <ctype.h>

// maintains the column definition i.e. its name and type
typedef struct
{
    // in future:: also add contraints member
    char col_name[100];
    char col_type[50];
} Column;

// maintains the table definition
typedef struct
{
    char table_name[35];
    Column columns[10];
    int num_columns;
} TableSchema;

int create_statement_parser(char tokens[][20], int num_of_tokens, TableSchema *table)
{
    table->num_columns = 0;
    int column_index = 0;
    int column_attribute_index = 0;

    if (strcasecmp(tokens[0], "create") != 0)
    {
        printf("Syntax error :: Expected CREATE statement.\n");
        return -1;
    }

    if (strcasecmp(tokens[1], "table") != 0)
    {
        printf("Syntax error :: Expected TABLE after CREATE.\n");
        return -1;
    }

    strcpy(table->table_name, tokens[2]); // add check to make sure table name is valid

    if (strcasecmp(tokens[3], "(") != 0)
    {
        printf("Syntax error :: Expected '(' after table name.");
        return -1;
    }

    // looping inside the (...) block. Pick each column name and its type that is being defined in the CREATE statement
    for (int i = 4; i < num_of_tokens; i++)
    // starting after opening bracket - [0]CREATE [1]TABLE [2]<table-name> [3](
    {
        if (strcasecmp(tokens[i], ")") == 0)
        {
            if (column_attribute_index == 2)
            {
                // a column definition was completed before
                table->num_columns++; // a column was completely defined prior to closure [name type])
                break;
            }

            printf("Syntax Error :: Expected column type before ')'");
            return -1;
        }

        if (column_attribute_index == 0) // populate the column_name member of the struct
        {
            strcpy(table->columns[column_index].col_name, tokens[i]);
            column_attribute_index++; // first struct member populated. Now the second will be populated
        }
        else if (column_attribute_index == 1) // populate the column_type member of the struct
        {
            strcpy(table->columns[column_index].col_type, tokens[i]);
            column_attribute_index++; // second struct member populated. Now comma expected. Move to next struct in the array
        }
        else if (column_attribute_index == 2 && strcasecmp(tokens[i], ",") == 0)
        {
            // comma encountered after column definition
            column_attribute_index = 0;
            column_index++;       // moving to new column
            table->num_columns++; // a column was completely defined [name type]
        }
        else
        {
            printf("Syntax Error :: Expected ',' or ')'");
            return -1;
        }
    }
    return 0;
}

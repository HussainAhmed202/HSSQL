#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "parser.h"
#include "catalog.h"

int main(void)
{
    char user_input[100] = {0}; // Limiting user input to 100 characters. Intialize empty array with 100 zeros
    char token[10][20] = {0};
    TableSchema table; // defined in the parser.h file
    TableSchema catalog[5] = {0};
    bool should_exit = false;
    int num_of_tables = 0;

    printf("Welcome to HSSQL\n");

    while (!should_exit)
    {
        printf("hssql> ");
        char *result = fgets(user_input, sizeof(user_input), stdin);
        // CTRL Z for Windows: Signal for EOF. Nothing input
        if (result == NULL)
        {
            // EOF signal sent
            printf("Goodbye\n");
            should_exit = true;
            continue; // loop again
        }

        // If fgets returns string that does not contain '\n' - this means that input is outside 100 Bytes limit
        if (strchr(user_input, '\n') == NULL)
        {
            printf("Command Max limit reached\n");

            // Clear the rest of the input
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            continue;
        }

        user_input[strcspn(user_input, "\n")] = '\0'; // strcspn gives the number of characters before \n. For example, H E L L O \N \0, this returns 5. We assign at pos 5, \0. This turns H E L L O \N \0 to H E L L O \0
        if (strcmp(user_input, ".exit") == 0)
        {
            printf("Goodbye\n");
            should_exit = true;
        }
        else
        {
            int token_count = tokenizer(token, user_input, (int)strlen(user_input));
            // printf("Total number of tokens: %d\n", token_count);
            // for (int i = 0; i < token_count; i++)
            // {
            //     printf("Token%d = ", i);
            //     for (int j = 0; j < 10; j++)
            //     {
            //         printf("%c", token[i][j]);
            //     }
            //     printf("\n");
            // }
            if (create_statement_parser(token, token_count, &table) == 0)
            {
                if (add_table_to_catalog(catalog, &num_of_tables, table) == -1)
                {
                    printf("Memory full - No more tables can be created");
                    return -1;
                }

                // printf("Table name: %s\n", table.table_name);
                // printf("Total number of columns %d\n", table.num_columns);

                // for (int i = 0; i < table.num_columns; i++)
                // {
                //     printf("Column[%zu].col_name -> %s\n", i, table.columns[i].col_name);
                //     printf("Column[%zu].col_type -> %s\n", i, table.columns[i].col_type);
                // }

                for (int i = 0; i < num_of_tables; i++)
                {
                    printf("Table name: %s\n", catalog[i].table_name);
                    printf("Total number of columns %d\n", catalog[i].num_columns);
                    for (int j = 0; j < catalog[i].num_columns; j++)
                    {
                        printf("Column[%d].col_name -> %s\n", i, catalog[i].columns[j].col_name);
                        printf("Column[%d].col_type -> %s\n", i, catalog[i].columns[j].col_type);
                    }
                }
            }
            else
            {
                printf("Something bad happened\n");
            }
        }
    }
    return 0;
}
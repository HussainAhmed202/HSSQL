#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "parser.h"
#include "catalog.h"

int main(void)
{
    char user_input[100] = {0}; // Limiting user input to 100 characters
    TableSchema catalog[5] = {0};
    TableSchema table;
    bool should_exit = false;
    int num_of_tables = 0;

    // re-intialize at each iteration as it holds new input
    char token[10][20] = {0}; // holds the tokenized user input

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
            if (create_statement_parser(token, token_count, &table) == -1)
            {
                printf("Something bad happended\n");
                // should_exit = true;
                continue;
            }

            /* Before inserting in the catalog, check if table already
            exists. If it is present, then throw error */
            TableSchema *found_table = find_table_by_name(catalog, num_of_tables, table.table_name);
            if (found_table != NULL)
            {
                // table found. Throw error.
                printf("Table %s already exists\n", found_table->table_name);
                // should_exit = true;
                continue;
            }
            /// table not present in the catalog.Add table to the catalog
            else if (add_table_to_catalog(catalog, &num_of_tables, table) == -1)
            {
                printf("Memory full - No more tables can be created");
                // should_exit = true;
                continue;
            }

            for (int i = 0; i < num_of_tables; i++)
            {
                printf("Table name: %s\n", catalog[i].table_name);
                printf("Total number of columns %d\n", catalog[i].num_columns);
                for (int j = 0; j < catalog[i].num_columns; j++)
                {
                    printf("Column[%d].col_name -> %s\n", j, catalog[i].columns[j].col_name);
                    printf("Column[%d].col_type -> %s\n", j, catalog[i].columns[j].col_type);
                }
            }
        }
    }

    return 0;
}
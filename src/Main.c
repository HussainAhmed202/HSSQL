#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "parser.h"
#include "catalog.h"
#include "repl.h"
#include "save.h"

int main(void)
{
    char in_stream[100] = {0};
    char token[10][20] = {0}; // holds the tokenized user input
    TableSchema table;
    TableSchema catalog[5] = {0};
    int num_of_tables = 0;

    welcome_msg();

    while (1)
    {
        printf("hssql> ");
        int reply = repl(in_stream, (int)sizeof(in_stream));

        if (reply == 1) // EOF or exit command
        {
            goodbye_msg();
            break;
        }
        else if (reply == 2)
        {
            printf("Command Max limit reached\n");
            continue;
        }

        int token_count = tokenizer(token, in_stream, (int)strlen(in_stream));
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
        else if (read_object("TABLE", table.table_name, &table, num_of_tables) == 10)
        {
            // Check in table in memory
            // table found. Throw error.
            printf("Table %s already exists\n", table.table_name);
            // should_exit = true;
            continue;
        }
        /// table not present in the catalog.Add table to the catalog
        else if (add_table_to_catalog(catalog, &num_of_tables, table) == -1)
        {
            printf("Memory full - No more tables can be created\n");
            // should_exit = true;
            continue;
        }
        else
        {
            // save the table in memory
            int written = save_object("TABLE", &catalog);
            if (written == 2)
            {
                printf("Table %s CREATED successfully\n", table.table_name);
                printf("Location %p\n", &catalog);
            }
            else if (written == 3)
            {
                printf("Error writing to file.\n");
            }
            else if (written == 1)
            {
                printf("Error opening file\n");
            }
            else
            {
                printf("Something issue happended\n");
            }
        }
    }
    return 0;
}
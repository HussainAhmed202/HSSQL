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

    Catalog catalog;
    catalog.num_of_tables = 0;
    catalog.max_capacity = 5;

    // user connects to a database.
    // that specific database catalog loaded.

    catalog.table_list = load_catalog_from_disk("db1", &catalog);
    // if (ptr == NULL)
    // {
    //     printf("Catalog file is empty\n");
    // }

    welcome_msg();

    while (1)
    {
        // printf("Catalog Table count = %d\n ", catalog.num_of_tables);

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
        // printf("Token count = %d\n", token_count);

        if (create_statement_parser(token, token_count, &table) == -1)
        {
            printf("Something bad happended\n");
            // should_exit = true;
            continue;
        }
        /* Before inserting in the catalog, check if table already
        exists. If it is present, then throw error */
        // printf("Table name = %s\n", table.table_name);

        TableSchema *found_table = find_table_in_catalog(&catalog, table.table_name);
        if (found_table != NULL)
        {
            // table found. Throw error.
            printf("Table %s already exists\n", found_table->table_name);
            // should_exit = true;
            continue;
        }
        // // Add table to the catalog
        // else if (add_table_to_catalog("db1", &catalog, table) == -1)
        // {
        //     printf("Issue storing table into catalog\n");
        //     // should_exit = true;
        //     continue;
        // }
        // else
        // {
        //     printf("Table %s CREATED successfully\n", table.table_name);
        //     continue;
        // }
    }
    return 0;
}
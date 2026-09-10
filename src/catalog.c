#include <stdio.h>
#include <string.h>
#include "parser.h"

#define MAX_TABLES 5

int add_table_to_catalog(TableSchema catalog[], int *num_tables, TableSchema new_table)
{
    if (*num_tables == MAX_TABLES)
    {
        return -1; // memory full
    }

    // returns the number of tables stored in the database catalog
    catalog[*num_tables] = new_table;
    return ++*num_tables;
}

TableSchema *find_table_by_name(TableSchema catalog[], int num_tables, const char table_name[])
{
    for (int i = 0; i < num_tables; i++)
    {
        if (strcmp(catalog[i].table_name, table_name) == 0)
        {
            return &catalog[i];
        }
    }
    return NULL;
}
// int main()
// {
//     int num_of_tables = 0;
//     TableSchema catalog[MAX_TABLES] = {0};

//     // CREATE TABLE user(NUMBER id, VARCHAR2 name)
//     TableSchema table1 = {
//         "user",
//         {{"id", "NUMBER"}, {"name", "VARCHAR2"}},
//         2};

//     if (add_table_to_catalog(catalog, &num_of_tables, table1) == -1)
//     {
//         printf("Memory full - No more tables can be created");
//         return -1;
//     }

//     // CREATE TABLE transactions(NUMBER id, DOUBLE amount)
//     TableSchema table2 = {
//         "transaction",
//         {{"id", "NUMBER"}, {"amount", "DOUBLE"}},
//         2};

//     if (add_table_to_catalog(catalog, &num_of_tables, table2) == -1)
//     {
//         printf("Memory full - No more tables can be created");
//         return -1;
//     }

//     // CREATE TABLE bank(NUMBER id, Number user_id, DOUBLE amount, VARCHAR2 user_name)
//     TableSchema table3 = {
//         "bank",
//         {{"id", "NUMBER"}, {"user_id", "NUMBER"}, {"amount", "DOUBLE"}, {"name", "VARCHAR2"}},
//         4};

//     if (add_table_to_catalog(catalog, &num_of_tables, table3) == -1)
//     {
//         printf("Memory full - No more tables can be created");
//         return -1;
//     }

//     for (int i = 0; i < num_of_tables; i++)
//     {
//         printf("Table name: %s\n", catalog[i].table_name);
//         printf("Total number of columns %d\n", catalog[i].num_columns);
//         for (int j = 0; j < catalog[i].num_columns; j++)
//         {
//             printf("Column[%d].col_name -> %s\n", i, catalog[i].columns[j].col_name);
//             printf("Column[%d].col_type -> %s\n", i, catalog[i].columns[j].col_type);
//         }
//     }

//     TableSchema *found_table = find_table_by_name(catalog, num_of_tables, "user");
//     if (found_table != NULL)
//     {
//         printf("Found: %s\n", found_table->table_name);
//     }
//     else
//     {
//         printf("Table not found\n");
//     }

//     return 0;
// }
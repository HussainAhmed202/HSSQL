#include <stdio.h>
#include <string.h>
#include "parser.h"

// #define MAX_TABLES 5

int add_table_to_catalog(TableSchema catalog[], int *num_tables, TableSchema new_table)
{
    // returns the number of tables stored in the database catalog
    catalog[*num_tables] = new_table;
    return ++*num_tables;
}

TableSchema *find_table_by_name(TableSchema catalog[], int num_tables, char table_name[])
{
    for (int i = 0; i < 2; i++)
    {
        if (strcmp(catalog[i].table_name, table_name) == 0)
        {
            return &catalog[i];
        }
    }
    return NULL;
}

int main()
{
    int num_of_tables = 0;
    TableSchema catalog[2] = {0};
    // CREATE TABLE user(NUMBER id, VARCHAR2 name)
    TableSchema table1 = {
        "user",
        {{"id", "NUMBER"}, {"name", "VARCHAR2"}},
        2};

    printf("%d\n", add_table_to_catalog(catalog, &num_of_tables, table1));

    // CREATE TABLE transactions(NUMBER id, DOUBLE amount)
    TableSchema table2 = {
        "transaction",
        {{"id", "NUMBER"}, {"amount", "DOUBLE"}},
        2};

    printf("%d\n", add_table_to_catalog(catalog, &num_of_tables, table2));

    for (int i = 0; i < 2; i++)
    {
        printf("Table name: %s\n", catalog[i].table_name);
        printf("Total number of columns %d\n", catalog[i].num_columns);
        for (int j = 0; j < catalog[i].num_columns; j++)
        {
            printf("Column[%zu].col_name -> %s\n", i, catalog[i].columns[j].col_name);
            printf("Column[%zu].col_type -> %s\n", i, catalog[i].columns[j].col_type);
        }
    }

    TableSchema *found_table = find_table_by_name(catalog, num_of_tables, "user");
    if (found_table != NULL)
    {
        printf("Found: %s\n", found_table->table_name);
    }
    else
    {
        printf("Table not found\n");
    }

    return 0;
}
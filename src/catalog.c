#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

TableSchema *load_catalog_from_disk(char *object_name, TableSchema *object)
{
    char file_path[512];

    int bytes_written = snprintf(file_path, sizeof(file_path), "data/db/%s/catalog.dat", object_name);
    if (bytes_written < 0 || bytes_written >= sizeof(file_path))
    {
        printf("Error: File path exceeds max buffer length.\n");
        return NULL;
    }

    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long total_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET); // Rewind back to the beginning

    // 2. Calculate the number of elements
    size_t count = total_bytes / sizeof(TableSchema);

    printf("Total file size: %ld bytes\n", total_bytes);
    printf("Number of tables: %zu\n", count);

    // 3. Allocate heap memory for the array of structs
    TableSchema *catalog = malloc(sizeof(TableSchema) * count);
    if (!catalog)
    {
        fclose(fp);
        return NULL;
    }

    // 4. Read array from disk
    fread(catalog, sizeof(TableSchema), count, fp);
    fclose(fp);

    return catalog;
}

int save_catalog_to_disk(char *object_name, TableSchema *object, size_t size)
{
    char file_path[512];

    int bytes_written = snprintf(file_path, sizeof(file_path), "data/db/%s/catalog.dat", object_name);
    if (bytes_written < 0 || bytes_written >= sizeof(file_path))
    {
        printf("Error: File path exceeds max buffer length.\n");
        return -1;
    }

    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        return -1;
    }

    size_t written = fwrite(&object, sizeof(TableSchema), size, fp);
    fclose(fp);
    return written;
}

int add_table_to_catalog(TableSchema catalog[], int *num_tables, TableSchema new_table)
{
    if (*num_tables == 5)
    {
        return -1; // memory full
    }

    // returns the number of tables stored in the database catalog
    catalog[*num_tables] = new_table;
    return ++*num_tables;
}

TableSchema *find_table_in_catalog(TableSchema catalog[], int num_tables, const char table_name[])
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
int main()
{
    int num_of_tables = 0;
    TableSchema *catalog = malloc(sizeof(TableSchema) * 5);

    // hssql>CREATE TABLE user(NUMBER id, VARCHAR2 name)
    TableSchema table1 = {
        "user",
        {{"id", "NUMBER"}, {"name", "VARCHAR2"}},
        2};

    if (add_table_to_catalog(catalog, &num_of_tables, table1) == -1)
    {
        printf("Memory full - No more tables can be created");
        return -1;
    }

    // CREATE TABLE transactions(NUMBER id, DOUBLE amount)
    TableSchema table2 = {
        "transaction",
        {{"id", "NUMBER"}, {"amount", "DOUBLE"}},
        2};

    if (add_table_to_catalog(catalog, &num_of_tables, table2) == -1)
    {
        printf("Memory full - No more tables can be created");
        return -1;
    }

    // CREATE TABLE bank(NUMBER id, Number user_id, DOUBLE amount, VARCHAR2 user_name)
    TableSchema table3 = {
        "bank",
        {{"id", "NUMBER"}, {"user_id", "NUMBER"}, {"amount", "DOUBLE"}, {"name", "VARCHAR2"}},
        4};

    if (add_table_to_catalog(catalog, &num_of_tables, table3) == -1)
    {
        printf("Memory full - No more tables can be created");
        return -1;
    }

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

    TableSchema *found_table = find_table_in_catalog(catalog, num_of_tables, "user");
    if (found_table != NULL)
    {
        printf("Found: %s\n", found_table->table_name);
    }
    else
    {
        printf("Table not found\n");
    }

    free(catalog);

    return 0;
}
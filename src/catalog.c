#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

TableSchema *load_catalog_from_disk(char *database_name, size_t *table_count)
{
    char file_path[512];

    int bytes_written = snprintf(file_path, sizeof(file_path), "data/db/%s/catalog.dat", database_name);
    if (bytes_written < 0 || bytes_written >= sizeof(file_path))
    {
        printf("Error: File path exceeds max buffer length.\n");
        *table_count = 0;
        return NULL;
    }

    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        *table_count = 0;
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long total_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET); // Rewind back to the beginning

    // 2. Calculate the number of elements
    size_t count = total_bytes / sizeof(TableSchema);

    printf("Total file size: %ld bytes\n", total_bytes);
    printf("Number of tables: %zu\n", count);

    TableSchema *catalog = malloc(sizeof(TableSchema) * count);
    if (!catalog)
    {
        fclose(fp);
        *table_count = 0;
        return NULL;
    }

    // 4. Read array from disk
    fread(catalog, sizeof(TableSchema), count, fp);
    fclose(fp);

    *table_count = count;
    return catalog;
}

int update_catalog_in_disk(char *database_name, TableSchema object)
{
    char file_path[512];

    int bytes_written = snprintf(file_path, sizeof(file_path), "data/db/%s/catalog.dat", database_name);
    if (bytes_written < 0 || bytes_written >= sizeof(file_path))
    {
        printf("Error: File path exceeds max buffer length.\n");
        return -1;
    }

    FILE *fp = fopen(file_path, "ab");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        return -1;
    }
    size_t written = fwrite(&object, sizeof(TableSchema), 1, fp);
    fclose(fp);
    return written;
}

int write_catalog_to_disk(char *database_name, TableSchema *object, size_t size)
{
    // for cleanup - Incase database gets corrupted. This function is called for re-creating the catalog
    char file_path[512];

    int bytes_written = snprintf(file_path, sizeof(file_path), "data/db/%s/catalog.dat", database_name);
    if (bytes_written < 0 || bytes_written >= sizeof(file_path))
    {
        printf("Error: File path exceeds max buffer length.\n");
        return -1;
    }

    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        return -1;
    }

    size_t written = fwrite(object, sizeof(TableSchema), size, fp);
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

    size_t num_of_tables = 0;

    // TableSchema *schema1 = malloc(sizeof(TableSchema) * 5);

    // // hssql>CREATE TABLE user(NUMBER id, VARCHAR2 name)
    // TableSchema table1 = {
    //     .table_name = "user",
    //     .columns = {
    //         {.col_name = "id", .col_type = "NUMBER"},
    //         {.col_name = "name", .col_type = "VARCHAR2"}},
    //     .num_columns = 2};

    // if (add_table_to_catalog(schema1, &num_of_tables, table1) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }

    // // CREATE TABLE transactions(NUMBER id, DOUBLE amount)
    // TableSchema table2 = {
    //     .table_name = "transaction",
    //     .columns = {{"id", "NUMBER"}, {"amount", "DOUBLE"}},
    //     .num_columns = 2};

    // if (add_table_to_catalog(schema1, &num_of_tables, table2) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }
    // // table successfully added to the catalog in memory. Not permanent.

    // if (update_catalog_in_disk("schema1", table2) != 1)
    // {
    //     printf("Error storing table to memory");
    //     return -1;
    // }
    // printf("Table Schema updated in memory");

    // CREATE TABLE bank(NUMBER id, Number user_id, DOUBLE amount, VARCHAR2 user_name)
    // TableSchema table3 = {
    //     .table_name = "bank",
    //     .columns = {{"id", "NUMBER"}, {"user_id", "NUMBER"}, {"amount", "DOUBLE"}, {"name", "VARCHAR2"}},
    //     .num_columns = 4};

    // if (add_table_to_catalog(schema1, &num_of_tables, table3) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }

    // // TableSchema *found_table = find_table_in_catalog(schema1, num_of_tables, "user");
    // // if (found_table != NULL)
    // // {
    // //     printf("Found: %s\n", found_table->table_name);
    // // }
    // // else
    // // {
    // //     printf("Table not found\n");
    // // }

    // if (write_catalog_to_disk("schema1", schema1, num_of_tables) == num_of_tables)
    // {
    //     printf("Success\n");
    // }
    // else
    // {
    //     printf("Fail\n");
    // }

    // free(schema1);

    TableSchema *schema1 = load_catalog_from_disk("schema1", &num_of_tables);
    if (schema1 != NULL)
        if (schema1 != NULL)
        {
            for (size_t i = 0; i < num_of_tables; i++)
            {
                printf("Table name: %s\n", schema1[i].table_name);
                printf("Total number of columns %d\n", schema1[i].num_columns);
                for (int j = 0; j < schema1[i].num_columns; j++)
                {
                    printf("Column[%d].col_name -> %s\n", i, schema1[i].columns[j].col_name);
                    printf("Column[%d].col_type -> %s\n", i, schema1[i].columns[j].col_type);
                }
            }

            //         TableSchema table4 = {
            //             "user",
            //             {{"id", "NUMBER"}, {"name", "VARCHAR2"}},
            //             2};

            //         if (add_table_to_catalog(schema1, &num_of_tables, table4) == -1)
            //         {
            //             printf("Memory full - No more tables can be created");
            //             return -1;
            //         }

            // if (write_catalog_to_disk("schema1", schema1, num_of_tables) == num_of_tables)
            // {
            //     printf("Success\n");
            // }
            // else
            // {
            //     printf("Fail\n");
            // }

            free(schema1);
        }

    return 0;
}
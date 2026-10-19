#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef struct Catalog
{
    int num_of_tables;
    int max_capacity;
    TableSchema *table_list;
} Catalog;

TableSchema *load_catalog_from_disk(char *database_name, Catalog *catalog)
{
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "data/%s/catalog.dat", database_name);
    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        catalog->num_of_tables = 0;
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long total_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET); // Rewind back to the beginning

    // 2. Calculate the number of elements
    size_t count = total_bytes / sizeof(TableSchema);
    printf("Total file size: %ld bytes\n", total_bytes);
    printf("Number of tables: %zu\n", count);

    if (total_bytes == 0)
    {
        // catalog file is empty
        // return catalog with an empty list of tables
        TableSchema *ptr = malloc(sizeof(TableSchema) * catalog->max_capacity);
        return ptr;
    }
    TableSchema *ptr = malloc(sizeof(TableSchema) * count);
    catalog->max_capacity = count;
    if (!ptr)
    {
        fclose(fp);
        catalog->num_of_tables = 0;
        return NULL;
    }
    // 4. Read array from disk
    fread(ptr, sizeof(TableSchema), count, fp);
    fclose(fp);

    catalog->num_of_tables = count;
    return ptr;
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

int add_table_to_catalog(char *database_name, Catalog *catalog, TableSchema new_table)
{
    char file_path[512];

    int bytes_written = snprintf(file_path, sizeof(file_path), "data/%s/catalog.dat", database_name);
    if (bytes_written < 0 || bytes_written >= sizeof(file_path))
    {
        printf("Error: File path exceeds max buffer length.\n");
        return -1;
    }

    if (catalog->num_of_tables == catalog->max_capacity)
    {
        // extend the in-memory catalog
        size_t new_count = catalog->max_capacity * 2;
        TableSchema *ptr = realloc(catalog->table_list, new_count * sizeof(TableSchema));
        if (ptr == NULL)
        {
            printf("Error in extending the catalog\n");
            return -1;
        }
        catalog->table_list = ptr; // new extended piece of memory
        ptr = NULL;
    }

    catalog->table_list[catalog->num_of_tables] = new_table;
    catalog->num_of_tables++;

    FILE *fp = fopen(file_path, "ab");
    if (fp == NULL)
    {
        printf("Failed to open catalog file '%s'\n", file_path);
        return -1;
    }

    size_t written = fwrite(&new_table, sizeof(TableSchema), 1, fp);
    fclose(fp);
    return written;
}

TableSchema *find_table_in_catalog(Catalog *catalog, char *table_name)
{
    for (int i = 0; i < catalog->num_of_tables; i++)
    {
        if (strcmp(catalog->table_list->table_name, table_name) == 0)
        {
            return &catalog->table_list[i];
        }
    }
    return NULL;
}

int main()
{
    Catalog catalog;
    catalog.num_of_tables = 0;
    catalog.max_capacity = 5;

    // user connects to a database.
    // that specific database catalog loaded.

    TableSchema *ptr = load_catalog_from_disk("db1", &catalog);
    if (ptr == NULL)
    {
        printf("Catalog file is empty\n");
    }
    catalog.table_list = ptr;
    ptr = NULL;

    // // hssql>CREATE TABLE user(NUMBER id, VARCHAR2 name)
    // TableSchema table1 = {
    //     .table_name = "user",
    //     .columns = {
    //         {.col_name = "id", .col_type = "NUMBER"},
    //         {.col_name = "name", .col_type = "VARCHAR2"}},
    //     .num_columns = 2};

    // if (add_table_to_catalog("db1", &catalog, table1) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }
    // printf("Table added\n");

    // // CREATE TABLE transactions(NUMBER id, DOUBLE amount)
    // TableSchema table2 = {
    //     .table_name = "transaction",
    //     .columns = {{"id", "NUMBER"}, {"amount", "DOUBLE"}},
    //     .num_columns = 2};

    // if (add_table_to_catalog("db1", &catalog, table2) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }
    // printf("Table added\n");

    // // CREATE TABLE bank(NUMBER id, Number user_id, DOUBLE amount, VARCHAR2 user_name)
    // TableSchema table3 = {
    //     .table_name = "bank",
    //     .columns = {{"id", "NUMBER"}, {"user_id", "NUMBER"}, {"amount", "DOUBLE"}, {"name", "VARCHAR2"}},
    //     .num_columns = 4};

    // if (add_table_to_catalog("db1", &catalog, table3) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }
    // printf("Table added\n");

    // if (add_table_to_catalog("db1", &catalog, table3) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }
    // printf("Table added\n");

    // if (add_table_to_catalog("db1", &catalog, table3) == -1)
    // {
    //     printf("Memory full - No more tables can be created");
    //     return -1;
    // }
    // printf("Table added\n");

    TableSchema *found_table = find_table_in_catalog(&catalog, "user");
    if (found_table != NULL)
    {
        printf("Found: %s\n", found_table->table_name);
    }
    else
    {
        printf("Table not found\n");
    }

    // else
    // {
    //     // table successfully added to the catalog in memory. Not permanent.
    //     if (append_table_catalog_in_disk("schema1", table1 != 1)
    //     {
    //         printf("Error storing table to memory");
    //         return -1;
    //     }
    //     printf("Table Schema updated in memory");
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
    // else
    // {
    //     // table successfully added to the catalog in memory. Not permanent.
    //     if (append_table_catalog_in_disk("schema1", table2) != 1)
    //     {
    //         printf("Error storing table to memory");
    //         return -1;
    //     }
    //     printf("Table Schema updated in memory");
    // }

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

    // if (write_catalog_to_disk("schema1", schema1, num_of_tables) == num_of_tables)
    // {
    //     printf("Success\n");
    // }
    // else
    // {
    //     printf("Fail\n");
    // }

    // free(schema1);

    //     TableSchema *schema1 = load_catalog_from_disk("schema1", &num_of_tables);
    //     if (schema1 != NULL)
    //     {
    //         // hssql > CREATE TABLE user(NUMBER id, VARCHAR2 name)
    //         TableSchema table1 = {
    //             .table_name = "user",
    //             .columns = {
    //                 {.col_name = "id", .col_type = "NUMBER"},
    //                 {.col_name = "name", .col_type = "VARCHAR2"}},
    //             .num_columns = 2};

    //         if (add_table_to_catalog(schema1, &num_of_tables, table1) == -1)
    //         {
    //             printf("Memory full - No more tables can be created");
    //             return -1;
    //         }
    //         printf("Table added to catalog in memory\n");

    //         // table successfully added to the catalog in memory. Not permanent.
    //         if (append_table_catalog_in_disk("schema1", &table1) == -1)
    //         {
    //             printf("Error storing table to memory");
    //             return -1;
    //         }
    //         printf("Table Schema updated in memory");
    //     }
    //     free(schema1);

    return 0;
}
#ifndef CATALOG_H
#define CATALOG_H

#define CATALOG_INITIAL_CAPACITY 5

typedef struct Catalog
{
    int num_of_tables;
    int max_capacity;
    TableSchema *table_list;
} Catalog;

/* Load the catalog from disk into memory */
TableSchema *load_catalog_from_disk(
    char *database_name,
    Catalog *catalog);

/*
    Called when a new database is created. This procedure will create the catalog file
*/
int create_catalog(const char *database_name);

/* Write the entire catalog to disk */
int write_catalog_to_disk(
    char *database_name,
    TableSchema *object,
    size_t size);

/* Add a table to the in-memory catalog and persist it */
int add_table_to_catalog(
    char *database_name,
    Catalog *catalog,
    TableSchema new_table);

/* Find a table by name */
TableSchema *find_table_in_catalog(
    Catalog *catalog,
    char *table_name);

#endif
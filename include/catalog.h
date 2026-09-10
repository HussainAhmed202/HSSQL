#ifndef PARSER_H
#define PARSER_H

// Maintains a column definition
typedef struct
{
    char col_name[100];
    char col_type[50];
} Column;

// Maintains the table/query definition
typedef struct
{
    char table_name[35];
    Column columns[10];
    int num_columns;
} TableSchema;

// Adds a table to the catalog.
// Returns the number of tables currently stored,
// or -1 if the catalog is full.
int add_table_to_catalog(TableSchema catalog[], int *num_tables, TableSchema new_table);

// Finds a table by name.
// Returns a pointer to the table if found,
// otherwise returns NULL.
TableSchema *find_table_by_name(TableSchema catalog[], int num_tables, const char table_name[]);
#endif
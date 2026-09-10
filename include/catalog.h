#ifndef CATALOG_H
#define CATALOG_H

#include "parser.h"

// Adds a table to the catalog.
// Returns the number of tables currently stored,
// or -1 if the catalog is full.
int add_table_to_catalog(TableSchema catalog[], int *num_tables, TableSchema new_table);

// Finds a table by name.
// Returns a pointer to the table if found,
// otherwise returns NULL.
TableSchema *find_table_by_name(TableSchema catalog[], int num_tables, const char table_name[]);
#endif
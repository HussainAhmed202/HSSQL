#ifndef CATALOG_H
#define CATALOG_H

#include "db.h"

/* Loads the catalog file (if present) into memory. Call once at startup. */
void catalog_init(void);

/* Returns the schema for a table, or NULL if it doesn't exist. */
TableSchema *catalog_get(const char *name);

/* Creates a new table: registers the schema, persists the catalog, and
 * creates an empty data file for it. Returns 0 on success, -1 if the
 * table already exists or the catalog is full. */
int catalog_create_table(const char *name, Column *columns, int num_columns);

/* Path helper: data/<table>.tbl */
void table_data_path(const char *table_name, char *out, size_t out_size);

#endif

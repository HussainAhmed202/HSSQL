#ifndef TABLE_H
#define TABLE_H

#include "db.h"

/* Appends one row to the table's data file. values must match the
 * schema's column order and count. Returns 0 on success. */
int table_insert(TableSchema *schema, Row *row);

/* Scans the table. If where_col is non-NULL, only rows where that column
 * equals where_val are passed to the callback. Callback receives the
 * decoded row and an opaque context pointer; return value is ignored.
 * Returns the number of rows visited (matching, if a filter is given). */
typedef void (*RowCallback)(Row *row, void *ctx);

int table_scan(TableSchema *schema, const char *where_col, const Value *where_val,
               RowCallback cb, void *ctx);

/* Deletes rows matching where_col = where_val (tombstones them).
 * Returns the number of rows deleted. */
int table_delete(TableSchema *schema, const char *where_col, const Value *where_val);

#endif

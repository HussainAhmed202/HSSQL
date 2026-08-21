#ifndef DB_H
#define DB_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TABLES        64
#define MAX_COLUMNS       16
#define MAX_NAME_LEN      32
#define MAX_TEXT_WIDTH    128
#define DATA_DIR          "data"
#define CATALOG_FILE      DATA_DIR "/catalog.db"

typedef enum {
    COL_INT,
    COL_TEXT
} ColType;

typedef struct {
    char    name[MAX_NAME_LEN];
    ColType type;
    int     width;      /* for COL_TEXT: max chars. for COL_INT: unused */
    int     offset;     /* byte offset within a row, computed at load time */
} Column;

typedef struct {
    char    name[MAX_NAME_LEN];
    int     num_columns;
    Column  columns[MAX_COLUMNS];
    int     row_size;   /* 1 (tombstone) + sum of column sizes */
} TableSchema;

/* A single cell value, tagged by type. Text is stored as a null-terminated
 * string with a max length of MAX_TEXT_WIDTH-1. */
typedef struct {
    ColType type;
    int64_t int_val;
    char    text_val[MAX_TEXT_WIDTH];
} Value;

/* One decoded row: values in column order */
typedef struct {
    Value values[MAX_COLUMNS];
    int   num_values;
} Row;

int  column_size(const Column *col);
int  find_column(const TableSchema *schema, const char *name);
void compute_layout(TableSchema *schema);

#endif

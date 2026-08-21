#include <string.h>
#include "db.h"

int column_size(const Column *col) {
    if (col->type == COL_INT) return sizeof(int64_t);
    return col->width; /* TEXT: fixed-width byte buffer, null padded */
}

int find_column(const TableSchema *schema, const char *name) {
    for (int i = 0; i < schema->num_columns; i++) {
        if (strcmp(schema->columns[i].name, name) == 0) return i;
    }
    return -1;
}

/* Lay out column byte offsets within a row. Byte 0 of every row is a
 * tombstone flag (1 = live, 0 = deleted), so column data starts at 1. */
void compute_layout(TableSchema *schema) {
    int offset = 1;
    for (int i = 0; i < schema->num_columns; i++) {
        schema->columns[i].offset = offset;
        offset += column_size(&schema->columns[i]);
    }
    schema->row_size = offset;
}

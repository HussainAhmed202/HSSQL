#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "catalog.h"

/* Encode a Row into a raw row buffer (caller-allocated, schema->row_size
 * bytes). byte 0 = tombstone (set to 1 = live). */
static void encode_row(TableSchema *schema, Row *row, uint8_t *buf) {
    buf[0] = 1; /* live */
    for (int i = 0; i < schema->num_columns; i++) {
        Column *col = &schema->columns[i];
        uint8_t *dest = buf + col->offset;
        if (col->type == COL_INT) {
            memcpy(dest, &row->values[i].int_val, sizeof(int64_t));
        } else {
            memset(dest, 0, col->width);
            strncpy((char *)dest, row->values[i].text_val, col->width - 1);
        }
    }
}

static void decode_row(TableSchema *schema, const uint8_t *buf, Row *row) {
    row->num_values = schema->num_columns;
    for (int i = 0; i < schema->num_columns; i++) {
        Column *col = &schema->columns[i];
        const uint8_t *src = buf + col->offset;
        row->values[i].type = col->type;
        if (col->type == COL_INT) {
            memcpy(&row->values[i].int_val, src, sizeof(int64_t));
        } else {
            strncpy(row->values[i].text_val, (const char *)src, MAX_TEXT_WIDTH - 1);
            row->values[i].text_val[MAX_TEXT_WIDTH - 1] = '\0';
        }
    }
}

static int values_equal(const Value *a, const Value *b) {
    if (a->type != b->type) return 0;
    if (a->type == COL_INT) return a->int_val == b->int_val;
    return strcmp(a->text_val, b->text_val) == 0;
}

int table_insert(TableSchema *schema, Row *row) {
    char path[256];
    table_data_path(schema->name, path, sizeof(path));
    FILE *f = fopen(path, "ab");
    if (!f) { perror("fopen"); return -1; }

    uint8_t *buf = calloc(1, schema->row_size);
    encode_row(schema, row, buf);
    size_t written = fwrite(buf, schema->row_size, 1, f);
    free(buf);
    fclose(f);

    return written == 1 ? 0 : -1;
}

int table_scan(TableSchema *schema, const char *where_col, const Value *where_val,
               RowCallback cb, void *ctx) {
    char path[256];
    table_data_path(schema->name, path, sizeof(path));
    FILE *f = fopen(path, "rb");
    if (!f) return 0; /* no data yet */

    int filter_idx = -1;
    if (where_col) {
        filter_idx = find_column(schema, where_col);
        if (filter_idx == -1) { fclose(f); return -1; }
    }

    uint8_t *buf = malloc(schema->row_size);
    int count = 0;
    Row row;
    while (fread(buf, schema->row_size, 1, f) == 1) {
        if (buf[0] != 1) continue; /* tombstoned */
        decode_row(schema, buf, &row);
        if (filter_idx >= 0 && !values_equal(&row.values[filter_idx], where_val)) continue;
        cb(&row, ctx);
        count++;
    }
    free(buf);
    fclose(f);
    return count;
}

int table_delete(TableSchema *schema, const char *where_col, const Value *where_val) {
    char path[256];
    table_data_path(schema->name, path, sizeof(path));
    FILE *f = fopen(path, "r+b");
    if (!f) return 0;

    int filter_idx = find_column(schema, where_col);
    if (filter_idx == -1) { fclose(f); return -1; }

    uint8_t *buf = malloc(schema->row_size);
    int deleted = 0;
    long pos = 0;
    Row row;
    while (fread(buf, schema->row_size, 1, f) == 1) {
        if (buf[0] == 1) {
            decode_row(schema, buf, &row);
            if (values_equal(&row.values[filter_idx], where_val)) {
                fseek(f, pos, SEEK_SET);
                uint8_t tomb = 0;
                fwrite(&tomb, 1, 1, f);
                fseek(f, pos + schema->row_size, SEEK_SET);
                deleted++;
            }
        }
        pos += schema->row_size;
    }
    free(buf);
    fclose(f);
    return deleted;
}

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "catalog.h"

static TableSchema g_tables[MAX_TABLES];
static int g_num_tables = 0;

void table_data_path(const char *table_name, char *out, size_t out_size) {
    snprintf(out, out_size, "%s/%s.tbl", DATA_DIR, table_name);
}

TableSchema *catalog_get(const char *name) {
    for (int i = 0; i < g_num_tables; i++) {
        if (strcmp(g_tables[i].name, name) == 0) return &g_tables[i];
    }
    return NULL;
}

/* Catalog file format, one line per table:
 *   tablename num_columns col1:TYPE:width col2:TYPE:width ...
 * width is 0 for INT columns.
 */
static void persist_catalog(void) {
    FILE *f = fopen(CATALOG_FILE, "w");
    if (!f) { perror("fopen catalog"); return; }
    for (int i = 0; i < g_num_tables; i++) {
        TableSchema *t = &g_tables[i];
        fprintf(f, "%s %d", t->name, t->num_columns);
        for (int c = 0; c < t->num_columns; c++) {
            Column *col = &t->columns[c];
            fprintf(f, " %s:%s:%d", col->name,
                    col->type == COL_INT ? "INT" : "TEXT",
                    col->type == COL_INT ? 0 : col->width);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void catalog_init(void) {
    mkdir(DATA_DIR, 0755); /* ignore error if it already exists */

    FILE *f = fopen(CATALOG_FILE, "r");
    if (!f) return; /* no catalog yet: fresh database */

    char line[2048];
    while (fgets(line, sizeof(line), f)) {
        TableSchema t;
        memset(&t, 0, sizeof(t));

        char *saveptr;
        char *tok = strtok_r(line, " \t\n", &saveptr);
        if (!tok) continue;
        strncpy(t.name, tok, MAX_NAME_LEN - 1);

        tok = strtok_r(NULL, " \t\n", &saveptr);
        if (!tok) continue;
        t.num_columns = atoi(tok);

        for (int c = 0; c < t.num_columns; c++) {
            tok = strtok_r(NULL, " \t\n", &saveptr);
            if (!tok) break;
            char cname[MAX_NAME_LEN], ctype[8];
            int width;
            sscanf(tok, "%31[^:]:%7[^:]:%d", cname, ctype, &width);
            strncpy(t.columns[c].name, cname, MAX_NAME_LEN - 1);
            t.columns[c].type = (strcmp(ctype, "INT") == 0) ? COL_INT : COL_TEXT;
            t.columns[c].width = width;
        }
        compute_layout(&t);
        g_tables[g_num_tables++] = t;
    }
    fclose(f);
}

int catalog_create_table(const char *name, Column *columns, int num_columns) {
    if (catalog_get(name)) return -1;
    if (g_num_tables >= MAX_TABLES) return -1;

    TableSchema *t = &g_tables[g_num_tables];
    memset(t, 0, sizeof(*t));
    strncpy(t->name, name, MAX_NAME_LEN - 1);
    t->num_columns = num_columns;
    for (int i = 0; i < num_columns; i++) t->columns[i] = columns[i];
    compute_layout(t);
    g_num_tables++;

    persist_catalog();

    /* create empty data file */
    char path[256];
    table_data_path(name, path, sizeof(path));
    FILE *f = fopen(path, "wb");
    if (!f) { perror("fopen data file"); return -1; }
    fclose(f);

    return 0;
}

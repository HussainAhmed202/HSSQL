#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* strcasecmp */
#include <ctype.h>
#include "sql.h"
#include "db.h"
#include "catalog.h"
#include "table.h"

#define MAX_TOKENS 256

typedef struct {
    char text[MAX_TEXT_WIDTH];
    int  is_string; /* was this token a quoted string literal? */
} Token;

/* ---------- Tokenizer ---------- */

static int tokenize(const char *sql, Token tokens[], int max_tokens) {
    int n = 0;
    const char *p = sql;
    while (*p) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        if (n >= max_tokens) break;

        if (*p == '\'' || *p == '"') {
            char quote = *p;
            p++;
            const char *start = p;
            while (*p && *p != quote) p++;
            size_t len = p - start;
            if (len >= MAX_TEXT_WIDTH) len = MAX_TEXT_WIDTH - 1;
            strncpy(tokens[n].text, start, len);
            tokens[n].text[len] = '\0';
            tokens[n].is_string = 1;
            n++;
            if (*p == quote) p++;
            continue;
        }

        if (strchr("(),;=*", *p)) {
            tokens[n].text[0] = *p;
            tokens[n].text[1] = '\0';
            tokens[n].is_string = 0;
            n++;
            p++;
            continue;
        }

        const char *start = p;
        while (*p && !isspace((unsigned char)*p) && !strchr("(),;=*", *p)) p++;
        size_t len = p - start;
        if (len >= MAX_TEXT_WIDTH) len = MAX_TEXT_WIDTH - 1;
        strncpy(tokens[n].text, start, len);
        tokens[n].text[len] = '\0';
        tokens[n].is_string = 0;
        n++;
    }
    return n;
}

static int is_kw(Token *t, const char *kw) {
    return !t->is_string && strcasecmp(t->text, kw) == 0;
}

/* ---------- CREATE TABLE ---------- */

static int parse_type(const char *text, ColType *type, int *width) {
    /* Accept "INT", "TEXT(20)" (already split into "TEXT" "(" "20" ")" by
     * the tokenizer since ( and ) are punctuation tokens; this function
     * handles the "TEXT" token only, caller consumes the (n) part). */
    if (strcasecmp(text, "INT") == 0) { *type = COL_INT; *width = 0; return 0; }
    if (strcasecmp(text, "TEXT") == 0) { *type = COL_TEXT; *width = 32; return 0; }
    return -1;
}

static int exec_create_table(Token *tok, int n, int i) {
    if (i >= n || tok[i].is_string) { printf("Error: expected table name\n"); return -1; }
    char table_name[MAX_NAME_LEN];
    strncpy(table_name, tok[i].text, MAX_NAME_LEN - 1);
    table_name[MAX_NAME_LEN-1] = '\0';
    i++;

    if (i >= n || strcmp(tok[i].text, "(") != 0) { printf("Error: expected '(' after table name\n"); return -1; }
    i++;

    Column columns[MAX_COLUMNS];
    int num_columns = 0;

    while (i < n && strcmp(tok[i].text, ")") != 0) {
        if (num_columns >= MAX_COLUMNS) { printf("Error: too many columns\n"); return -1; }
        if (tok[i].is_string) { printf("Error: expected column name\n"); return -1; }
        strncpy(columns[num_columns].name, tok[i].text, MAX_NAME_LEN - 1);
        columns[num_columns].name[MAX_NAME_LEN-1] = '\0';
        i++;

        if (i >= n) { printf("Error: expected column type\n"); return -1; }
        ColType type; int width;
        if (parse_type(tok[i].text, &type, &width) != 0) {
            printf("Error: unknown type '%s' (use INT or TEXT)\n", tok[i].text);
            return -1;
        }
        i++;

        if (type == COL_TEXT && i < n && strcmp(tok[i].text, "(") == 0) {
            i++; /* consume ( */
            if (i >= n) { printf("Error: expected width in TEXT(n)\n"); return -1; }
            width = atoi(tok[i].text);
            if (width <= 0 || width >= MAX_TEXT_WIDTH) width = 32;
            i++;
            if (i >= n || strcmp(tok[i].text, ")") != 0) { printf("Error: expected ')' after TEXT width\n"); return -1; }
            i++; /* consume ) */
        }

        columns[num_columns].type = type;
        columns[num_columns].width = width;
        num_columns++;

        if (i < n && strcmp(tok[i].text, ",") == 0) i++;
    }

    if (num_columns == 0) { printf("Error: table must have at least one column\n"); return -1; }

    if (catalog_create_table(table_name, columns, num_columns) != 0) {
        printf("Error: table '%s' already exists or catalog is full\n", table_name);
        return -1;
    }

    printf("Table '%s' created.\n", table_name);
    return 0;
}

/* ---------- INSERT ---------- */

static int exec_insert(Token *tok, int n, int i) {
    if (!is_kw(&tok[i], "INTO")) { printf("Error: expected INTO\n"); return -1; }
    i++;

    if (i >= n) { printf("Error: expected table name\n"); return -1; }
    TableSchema *schema = catalog_get(tok[i].text);
    if (!schema) { printf("Error: no such table '%s'\n", tok[i].text); return -1; }
    i++;

    if (i >= n || !is_kw(&tok[i], "VALUES")) { printf("Error: expected VALUES\n"); return -1; }
    i++;

    if (i >= n || strcmp(tok[i].text, "(") != 0) { printf("Error: expected '('\n"); return -1; }
    i++;

    Row row;
    row.num_values = 0;

    while (i < n && strcmp(tok[i].text, ")") != 0) {
        if (row.num_values >= schema->num_columns) { printf("Error: too many values\n"); return -1; }
        Column *col = &schema->columns[row.num_values];
        Value *v = &row.values[row.num_values];
        v->type = col->type;
        if (col->type == COL_INT) {
            v->int_val = atoll(tok[i].text);
        } else {
            strncpy(v->text_val, tok[i].text, MAX_TEXT_WIDTH - 1);
            v->text_val[MAX_TEXT_WIDTH - 1] = '\0';
        }
        row.num_values++;
        i++;
        if (i < n && strcmp(tok[i].text, ",") == 0) i++;
    }

    if (row.num_values != schema->num_columns) {
        printf("Error: expected %d values, got %d\n", schema->num_columns, row.num_values);
        return -1;
    }

    if (table_insert(schema, &row) != 0) { printf("Error: insert failed\n"); return -1; }
    printf("1 row inserted.\n");
    return 0;
}

/* ---------- SELECT ---------- */

typedef struct {
    int proj_cols[MAX_COLUMNS]; /* indices into schema->columns to print */
    int num_proj;               /* 0 means "all columns" */
    int rows_printed;
} PrintCtx;

static void print_row(Row *row, void *ctx_v) {
    PrintCtx *ctx = ctx_v;
    int n = ctx->num_proj > 0 ? ctx->num_proj : row->num_values;
    for (int k = 0; k < n; k++) {
        int idx = ctx->num_proj > 0 ? ctx->proj_cols[k] : k;
        if (k > 0) printf(" | ");
        Value *v = &row->values[idx];
        if (v->type == COL_INT) printf("%lld", (long long)v->int_val);
        else printf("%s", v->text_val);
    }
    printf("\n");
    ctx->rows_printed++;
}

/* Parses "col = literal" (used for WHERE). Returns 0 on success. */
static int parse_where(Token *tok, int n, int i, TableSchema *schema,
                        char *out_col, Value *out_val) {
    if (i >= n) { printf("Error: expected column after WHERE\n"); return -1; }
    strncpy(out_col, tok[i].text, MAX_NAME_LEN - 1);
    out_col[MAX_NAME_LEN-1] = '\0';
    int col_idx = find_column(schema, out_col);
    if (col_idx == -1) { printf("Error: no such column '%s'\n", out_col); return -1; }
    i++;

    if (i >= n || strcmp(tok[i].text, "=") != 0) { printf("Error: expected '=' in WHERE clause\n"); return -1; }
    i++;

    if (i >= n) { printf("Error: expected value after '='\n"); return -1; }
    Column *col = &schema->columns[col_idx];
    out_val->type = col->type;
    if (col->type == COL_INT) out_val->int_val = atoll(tok[i].text);
    else {
        strncpy(out_val->text_val, tok[i].text, MAX_TEXT_WIDTH - 1);
        out_val->text_val[MAX_TEXT_WIDTH - 1] = '\0';
    }
    return 0;
}

static int exec_select(Token *tok, int n, int i) {
    char proj_names[MAX_COLUMNS][MAX_NAME_LEN];
    int num_proj = 0;
    int select_all = 0;

    if (i < n && strcmp(tok[i].text, "*") == 0) {
        select_all = 1;
        i++;
    } else {
        while (i < n && !is_kw(&tok[i], "FROM")) {
            if (num_proj >= MAX_COLUMNS) { printf("Error: too many columns\n"); return -1; }
            strncpy(proj_names[num_proj], tok[i].text, MAX_NAME_LEN - 1);
            proj_names[num_proj][MAX_NAME_LEN-1] = '\0';
            num_proj++;
            i++;
            if (i < n && strcmp(tok[i].text, ",") == 0) i++;
        }
    }

    if (i >= n || !is_kw(&tok[i], "FROM")) { printf("Error: expected FROM\n"); return -1; }
    i++;

    if (i >= n) { printf("Error: expected table name\n"); return -1; }
    TableSchema *schema = catalog_get(tok[i].text);
    if (!schema) { printf("Error: no such table '%s'\n", tok[i].text); return -1; }
    i++;

    PrintCtx ctx;
    ctx.rows_printed = 0;
    if (select_all) {
        ctx.num_proj = 0;
    } else {
        ctx.num_proj = num_proj;
        for (int k = 0; k < num_proj; k++) {
            int idx = find_column(schema, proj_names[k]);
            if (idx == -1) { printf("Error: no such column '%s'\n", proj_names[k]); return -1; }
            ctx.proj_cols[k] = idx;
        }
    }

    const char *where_col = NULL;
    char where_col_buf[MAX_NAME_LEN];
    Value where_val;

    if (i < n && is_kw(&tok[i], "WHERE")) {
        i++;
        if (parse_where(tok, n, i, schema, where_col_buf, &where_val) != 0) return -1;
        where_col = where_col_buf;
    }

    /* print header */
    int hdr_n = ctx.num_proj > 0 ? ctx.num_proj : schema->num_columns;
    for (int k = 0; k < hdr_n; k++) {
        int idx = ctx.num_proj > 0 ? ctx.proj_cols[k] : k;
        if (k > 0) printf(" | ");
        printf("%s", schema->columns[idx].name);
    }
    printf("\n");
    for (int k = 0; k < hdr_n * 8; k++) printf("-");
    printf("\n");

    int visited = table_scan(schema, where_col, where_col ? &where_val : NULL, print_row, &ctx);
    if (visited < 0) { printf("Error: scan failed\n"); return -1; }
    printf("(%d row%s)\n", ctx.rows_printed, ctx.rows_printed == 1 ? "" : "s");
    return 0;
}

/* ---------- DELETE ---------- */

static int exec_delete(Token *tok, int n, int i) {
    if (i >= n || !is_kw(&tok[i], "FROM")) { printf("Error: expected FROM\n"); return -1; }
    i++;

    if (i >= n) { printf("Error: expected table name\n"); return -1; }
    TableSchema *schema = catalog_get(tok[i].text);
    if (!schema) { printf("Error: no such table '%s'\n", tok[i].text); return -1; }
    i++;

    if (i >= n || !is_kw(&tok[i], "WHERE")) {
        printf("Error: DELETE without WHERE is not supported (safety net) — "
               "add a WHERE clause.\n");
        return -1;
    }
    i++;

    char where_col_buf[MAX_NAME_LEN];
    Value where_val;
    if (parse_where(tok, n, i, schema, where_col_buf, &where_val) != 0) return -1;

    int deleted = table_delete(schema, where_col_buf, &where_val);
    if (deleted < 0) { printf("Error: delete failed\n"); return -1; }
    printf("%d row%s deleted.\n", deleted, deleted == 1 ? "" : "s");
    return 0;
}

/* ---------- Dispatcher ---------- */

int sql_execute(const char *sql) {
    Token tokens[MAX_TOKENS];
    int n = tokenize(sql, tokens, MAX_TOKENS);
    if (n == 0) return 0;

    int i = 0;
    if (is_kw(&tokens[i], "CREATE")) {
        i++;
        if (i >= n || !is_kw(&tokens[i], "TABLE")) { printf("Error: expected TABLE\n"); return -1; }
        i++;
        return exec_create_table(tokens, n, i);
    }
    if (is_kw(&tokens[i], "INSERT")) {
        i++;
        return exec_insert(tokens, n, i);
    }
    if (is_kw(&tokens[i], "SELECT")) {
        i++;
        return exec_select(tokens, n, i);
    }
    if (is_kw(&tokens[i], "DELETE")) {
        i++;
        return exec_delete(tokens, n, i);
    }
    printf("Error: unrecognized statement (try CREATE TABLE, INSERT, SELECT, DELETE)\n");
    return -1;
}

#include <stdio.h>
#include <string.h>
#include "catalog.h"
#include "sql.h"

int main(int argc, char **argv) {
    catalog_init();

    printf("minidb - a tiny relational database engine\n");
    printf("Statements: CREATE TABLE, INSERT INTO, SELECT, DELETE FROM. Type .exit to quit.\n\n");

    /* If a .sql file is passed, run it non-interactively; otherwise REPL. */
    FILE *input = stdin;
    int interactive = 1;
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) { perror("fopen"); return 1; }
        interactive = 0;
    }

    char line[4096];
    while (1) {
        if (interactive) printf("minidb> ");
        if (!fgets(line, sizeof(line), input)) break;

        /* strip trailing newline / semicolon */
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r' ||
                            line[len-1] == ';' || line[len-1] == ' ')) {
            line[--len] = '\0';
        }
        if (len == 0) continue;

        if (strcmp(line, ".exit") == 0 || strcmp(line, ".quit") == 0) break;

        sql_execute(line);
    }

    if (!interactive) fclose(input);
    return 0;
}

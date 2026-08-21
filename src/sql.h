#ifndef SQL_H
#define SQL_H

/* Parses and executes one SQL statement, printing results/errors to
 * stdout. Returns 0 on success, -1 on parse/execution error. */
int sql_execute(const char *sql);

#endif

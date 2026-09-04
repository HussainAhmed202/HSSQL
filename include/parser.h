#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Maintains the column definition, i.e. its name and type
typedef struct
{
    // In future: also add constraints member
    char col_name[100];
    char col_type[50];

} Column;

// Maintains the table/query definition
typedef struct
{
    char table_name[35];
    Column columns[10];
    int num_columns;

} Query;

// Parses a CREATE TABLE statement
int create_statement_parser(char tokens[][20],
                            int num_of_tokens,
                            Query *query);

#endif
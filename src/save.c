#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "catalog.h"

int read_object(char *object_type, char *object_name, void *object, void *objects_array, int num_of_elements_objects_array)
{
    // here there will be if statement that will deduce what kind of object this is
    // rn, only tables
    if (object_type == "TABLE")
    {
        FILE *read_ptr = fopen("data/table.dat", "rb");
        if (read_ptr == NULL)
        {
            perror("Error opening file for reading");
            return EXIT_FAILURE;
        }
        long offset = 0;

        for (int i = 0; i < num_of_elements_objects_array; i++)
        {
            offset = (long)i * sizeof(TableSchema);
            if (fseek(read_ptr, offset, SEEK_SET) != 0)
            {
                perror("Error seeking to location");
                fclose(read_ptr);
                return 0;
            }
            else
            {
                size_t item_read = fread(object, sizeof(TableSchema), 1, read_ptr);
                if (item_read != 1)
                {
                    printf("Error reading data from file.\n");
                    return EXIT_SUCCESS;
                }
                TableSchema table_object = *((TableSchema *)object);
                if (strcmp(table_object.table_name, object_name) == 0)
                {
                    printf("Table name = %s\n", table_object.table_name);
                    printf("----------------------------------\n");
                    printf("NAME\t|\tTYPE\n");
                    for (int j = 0; j < table_object.num_columns; j++)
                    {
                        printf("%s\t|\t%s\n", table_object.columns[j].col_name, table_object.columns[j].col_type);
                    }
                    printf("----------------------------------\n");
                    return 1;
                }
            }
        }
        fclose(read_ptr);

        // if program execution is in this block it means table not found
        printf("Error:: No table with the name %s found\n", object_name);
        return -1;
    }
}

int save_object(char *object_type, void *object)
{
    // here there will be if statement that will deduce what kind of object
    // to typecast to based on the object type provided

    if (object_type == "TABLE")
    {
        TableSchema table_object = *((TableSchema *)object);
        FILE *file = fopen("data/table.dat", "ab");
        if (file == NULL)
        {
            return 1;
        }

        size_t written = fwrite(&table_object, sizeof(TableSchema), 1, file);

        if (written == 1)
        {
            fclose(file);
            return 2;
        }
        else
        {
            fclose(file);
            return 3;
        }
    }
}

int main()
{
    Column table_1_col_1 = {"id", "NUMBER"};
    Column table_1_col_2 = {"name", "VARCHAR2"};

    TableSchema table1 = {
        "users",
        {table_1_col_1, table_1_col_2},
        2};
    TableSchema catalog[1] = {table1};

    // int written = save_object("TABLE", &catalog);
    // if (written == 2)
    // {
    //     printf("Struct written to file successfully.\n");
    // }
    // else if (written == 3)
    // {
    //     printf("Error writing struct to file.\n");
    // }
    // else if (written == 1)
    // {
    //     printf("Error opening file\n");
    // }
    // else
    // {
    //     printf("Something issue happended\n");
    // }

    TableSchema table2; //  read this table

    read_object("TABLE", "users", &table2, &catalog, 1);

    return 0;
}
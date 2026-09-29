#include <stdio.h>

typedef struct table
{
    int num_of_col;
    char name[50];
} table;

int main()
{
    table t1 = {2, "users"};

    FILE *file = fopen("data/table.dat", "wb");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    size_t written = fwrite(&t1, sizeof(t1), 1, file);

    if (written == 1)
    {
        printf("Struct written to file successfully.\n");
    }
    else
    {
        printf("Error writing struct to file.\n");
    }

    // 5. Always close the file
    fclose(file);

    return 0;
}
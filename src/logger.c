#include <stdio.h>
#include <string.h>

int wal_log(char *message)
{
    FILE *file = fopen("Logs/WAL.log", "log");
    if (file == NULL)
    {
        return 1;
    }

    int written = fwrite(&message, sizeof(message), strlen(message), file);

    if (strlen(message) != written)
    {
        printf("Error logging into WAL\n");
        fclose(file);
        return -1;
    }
    printf("Logged successfully\n");
    fclose(file);
}

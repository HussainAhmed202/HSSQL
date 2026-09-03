#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
int main(void)
{
    char user_input[100] = {0}; // Limiting user input to 100 characters. Intialize empty array with 100 zeros
    char token[10][10] = {0};
    bool should_exit = false;
    printf("Welcome to HSSQL\n");

    while (!should_exit)
    {
        printf("hssql> ");
        char *result = fgets(user_input, sizeof(user_input), stdin);
        // CTRL Z for Windows: Signal for EOF. Nothing input
        if (result == NULL)
        {
            // EOF signal sent
            printf("Goodbye\n");
            should_exit = true;
            continue; // loop again
        }

        // If fgets returns string that does not contain '\n' - this means that input is outside 100 Bytes limit
        if (strchr(user_input, '\n') == NULL)
        {
            printf("Command Max limit reached\n");

            // Clear the rest of the input
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            continue;
        }

        user_input[strcspn(user_input, "\n")] = '\0'; // strcspn gives the number of characters before \n. For example, H E L L O \N \0, this returns 5. We assign at pos 5, \0. This turns H E L L O \N \0 to H E L L O \0
        if (strcmp(user_input, ".exit") == 0)
        {
            printf("Goodbye\n");
            should_exit = true;
        }
        else
        {
            int token_count = tokenizer(token, user_input, (int)strlen(user_input));
            // printf("Total number of tokens: %d\n", token_count);
            for (int i = 0; i < token_count; i++)
            {
                printf("Token%d = ", i);
                for (int j = 0; j < 10; j++)
                {
                    printf("%c", token[i][j]);
                }
                printf("\n");
            }
        }
    }
    return 0;
}
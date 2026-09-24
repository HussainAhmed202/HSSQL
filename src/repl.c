#include <stdio.h>
#include <string.h>
// #include "tokenizer.h"
// #include "parser.h"
// #include "catalog.h"

void welcome_msg()
{
    printf("WELCOME TO HSSQL\n");
    printf("Database for no one\n");
    printf("---------------------------------\n");
    printf("For help type --help\n");
    printf("---------------------------------\n");
}

void goodbye_msg()
{
    printf("---------------------------------\n");
    printf("Goodbye\n");
    printf("---------------------------------\n");
}

int is_valid_input(char *user_input, int max_input_size)
{
    // EOF signal sent
    return (user_input == NULL) == 1;

    // If fgets returns string that does not contain '\n' - this means that input is outside 100 Bytes limit
    if (strchr(user_input, '\n') == NULL)
    {
        printf("Command Max limit reached\n");

        // Clear the rest of the input
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }

    // Remove new-line character
    user_input[strcspn(user_input, "\n")] = '\0';
    return (strcmp(user_input, ".exit") == 0);

    // all cases passed - input is valid
    return 1;
}

int main()
{
    char user_input[10] = {0}; // Limiting user input to 100 characters
    // char token[10][20] = {0};  // holds the tokenized user input
    // TableSchema table;
    welcome_msg();
    printf("hssql> ");
    if (!is_valid_input(fgets(user_input, 10, stdin), 10))
    {
        goodbye_msg();
    }

    // printf("%s\n", user_input);
    // if (is_valid_input(user_input, 10))
    // {
    //     printf("%s\n", user_input);
    // }

    // while ()
    // {
    //     // Remove new-line character
    //     user_input[strcspn(user_input, "\n")] = '\0';
    //     int token_count = tokenizer(token, user_input, 10);
    //     if (create_statement_parser(token, token_count, &table) == -1)
    //     {
    //         printf("Something bad happended\n");
    //         // should_exit = true;
    //         continue;
    //     }
    // }
    return 0;
}
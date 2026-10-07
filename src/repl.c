#include <stdio.h>
#include <string.h>

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

int is_input_length_valid(char *input)
{
    if (strchr(input, '\n') == NULL)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return 0;
    }

    return 1;
}

int repl(char *in_stream, int max_input_len)
{
    char *user_input = fgets(in_stream, max_input_len, stdin);

    // EOF
    if (user_input == NULL)
    {
        return 1;
    }

    // Input length exceeds limit
    if (!is_input_length_valid(user_input))
    {
        return 2;
    }

    // Remove newline
    user_input[strcspn(user_input, "\n")] = '\0';

    // .exit
    if (strcmp(user_input, ".exit") == 0)
    {
        return 1;
    }

    return 0;
}

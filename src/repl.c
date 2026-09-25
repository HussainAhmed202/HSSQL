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

int is_EOF(char *input)
{
    if (input == NULL)
    {
        return 1;
    }

    return 0;
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

void trim_input(char *input)
{
    input[strcspn(input, "\n")] = '\0';
}

int is_exit_command(char *input)
{
    if (strcmp(input, ".exit") == 0)
    {
        return 1;
    }

    return 0;
}

int repl(char *in_stream, int max_input_len)
{
    char *user_input = fgets(in_stream, max_input_len, stdin);

    // EOF
    if (is_EOF(user_input))
    {
        return 1;
    }

    // Input length exceeds limit
    if (!is_input_length_valid(user_input))
    {
        printf("Command Max limit reached\n");
    }

    // Remove newline
    trim_input(user_input);

    // .exit
    if (is_exit_command(user_input))
    {
        return 1;
    }

    return 0;
}

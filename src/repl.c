#include <stdio.h>

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

int repl(char *user_input, int max_input_size)
{
    printf("hssql> ");
    char *result = fgets(user_input, max_input_size, stdin);
    // CTRL Z for Windows: Signal for EOF. Nothing input
    if (result == NULL)
    {
        // EOF signal sent
        return 0;
    }
    return 1;
}

int main()
{
    char user_input[100] = {0}; // Limiting user input to 100 characters
    welcome_msg();
    while (repl(user_input, 100))
    {
        /* code */
    }
    goodbye_msg();
    return 0;
}
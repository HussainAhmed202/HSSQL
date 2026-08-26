#include <stdio.h>
#include <string.h>

int main()
{
    char string[] = "SELECT * FROM users";
    char token[10][10] = {0}; // stores one key word at a time
    int token_count = 0;
    int i = 0, j = 0, k = 0; // i is the token#, j is the index of string,  k is the column#

    printf("Input string: %s\n", string);

    printf("Length of the string: %zu\n", strlen(string));

    for (j = 0; j < strlen(string); j++)
    {
        if (string[j] == ' ' && k > 0)
        {
            // Note:: Since k=0; Space was encountered on previous iteration
            // No text was being parsed i.e. we are between tokens.

            i++;   // new word so next token
            k = 0; // marks the 0th index of the new token
            token_count++;
        }
        else if (string[j] == ' ')
        {
            continue;
        }
        else if ((
                     string[j] == '=' || string[j] == '+' || string[j] == '-' || string[j] == '/' || string[j] == '*' || string[j] == '(' || string[j] == ')' || string[j] == ';' || string[j] == ',') &&
                 k > 0)
        {
            // we were parsing a word. Then encountered a special char.

            // finalize the word first. Then the special char
            i++;
            k = 0;
            token_count++;

            // next token is the special char
            token[i][k] = string[j];
            token_count++;

            // move to the next token
            i++;
        }
        else if ((
                     string[j] == '=' || string[j] == '+' || string[j] == '-' || string[j] == '/' || string[j] == '*' || string[j] == '(' || string[j] == ')' || string[j] == ';' || string[j] == ',') &&
                 k == 0)
        {
            // we encountered space in prev iteration.
            // already i++; k reset k= 0;
            token[i][k] = string[j];

            // move to the next token
            i++;
            token_count++;
        }
        else
        {
            token[i][k] = string[j];
            k++;
        }
    }

    if (k > 0)
    {
        // there is one token which was not counted in the loop
        // this could be because there is only one word in the input
        // the last word of the input
        // k was never reset for both cases
        token_count++;
    }

    printf("Total number of tokens: %d\n", token_count);
    for (i = 0; i < token_count; i++)
    {
        printf("Token%d = ", i);
        for (j = 0; j < 10; j++)
        {
            printf("%c", token[i][j]);
        }
        printf("\n");
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

static int is_special_char(char c)
{
    return c == '=' || c == '+' || c == '-' || c == '/' || c == '*' || c == '(' || c == ')' || c == ';' || c == ',';
}

int tokenizer(char tokens[][10], char user_input[], int user_input_length)
{
    int token_count = 0;
    int token_index = 0; // Which token am I currently building?
    int char_index = 0;  // Which character of that token am I writing

    for (int j = 0; j < user_input_length; j++)
    {
        // white space region. No text encountered
        if (user_input[j] == ' ' && char_index == 0)
        {
            // Note:: Since char_index=0; Space was encountered on previous iteration
            // No text was being read i.e. we are between tokens.
            continue;
        }

        // below is code active when text is present
        if (user_input[j] == ' ' && char_index > 0)
        {
            // space encountered.
            // char_index > 0 i.e. a token was being read
            // white space marks the end of the token
            token_index++;  // new word so next token
            char_index = 0; // marks the 0th index of the new token
            token_count++;
        }
        else if ((is_special_char(user_input[j])) && char_index > 0)
        {
            // we were reading a word. Then encountered a special character

            // finalize the word.
            token_index++;
            char_index = 0;
            token_count++;

            // the special character is a separate token
            tokens[token_index][char_index] = user_input[j];
            token_count++;

            // move to the next tokens
            token_index++;
        }
        else if ((is_special_char(user_input[j])) && char_index == 0)
        {
            // we encountered space in prev iteration.
            // already token_index++; char_index reset char_index= 0;
            tokens[token_index][char_index] = user_input[j];

            // move to the next tokens
            token_index++;
            token_count++;
        }
        else
        {
            // Normal text. No special characters
            tokens[token_index][char_index] = user_input[j];
            char_index++;
        }
    }

    if (char_index > 0)
    {
        // there is one tokens which was not counted in the loop
        // this could be because there is only one word in the input
        // the last word of the input
        // char_index was never reset for both cases
        token_count++;
    }

    return token_count;
}

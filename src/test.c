#include <stdio.h>
#include <string.h>

int main()
{
    char string[] = "CREATE TABLE CUSTOMER(INT a1, BOOL Flag)";
    char token[10][10] = {0}; // stores one key word at a time
    int i = 0, j = 0, k = 0;  // i is the token#, j is the index of string,  k is the column#

    printf("Length of the string: %zu\n", strlen(string));

    for (j = 0; j < strlen(string); j++)
    {
        // printf("i=%d j=%d k=%d string[%d]=%c\n", i, j, k, j, string[j]);
        // intially i = 0; once ' ' encountered i++
        if (string[j] == ' ')
        {
            i++;   // new word so next token
            k = 0; // marks the 0th index of the new token
        }
        else if (string[j] == '=' || string[j] == '+' || string[j] == '-' || string[j] == '/' || string[j] == '*' || string[j] == '(' || string[j] == ')' || string[j] == ';' || string[j] == ',')
        {
            // Other special characters like =+-/*();
            if (k == 0)
            {
                // after space encountered, i incremented and k reset already
                token[i][k] = string[j];
                /// special characters stored as separate token
                i++;
                k = 0;
            }
            else
            {
                // special character encountered after a text character
                /// special characters stored as separate token
                i++;
                k = 0;
                token[i][k] = string[j];
                i++;
                k = 0;
            }
        }

        else
        {
            // i = 0 j = 0 k = 0 string[0]→ token[0][0] = S
            // i = 0 j = 1 k = 1 string[1]→ token[0][1] = E
            // i = 0 j = 2 k = 2 string[2]→ token[0][2] = L
            // i = 0 j = 3 k = 3 string[3]→ token[0][3] = E
            // i = 0 j = 4 k = 4 string[4]→ token[0][4] = C
            // i = 0 j = 5 k = 5 string[5]→ token[0][4] = T

            // i = 1 j = 6 k = 0 → space → reset k

            // i = 1 j = 7 k = 0 string[7] → token[1][0] = (

            // i = 2 j = 8 k = 0 string[8] → token[2][0] = 1

            // i = 3 j = 9 k = 0 string[9] → token[3][0] = +

            // i = 4 j = 10 k = 0 string[10] → token[4][0] = 2

            // i = 5 j = 11 k = 0 string[11] → token[5][0] = )

            // i = 6 j = 12 k = 0 space → reset k

            // i = 7 j = 13 k = 0  string[13] → token[7][0] = F
            // i = 7 j = 14 k = 1  string[14] → token[7][1] = R
            // i = 7 j = 15 k = 2  string[15] → token[7][2] = O
            // i = 7 j = 16 k = 3  string[16] → token[7][3] = M

            // i = 8 j = 17 k = 0  space → reset k

            // i = 8 j = 18 k = 0  string[18] → token[8][0] = D
            // i = 8 j = 19 k = 1  string[19] → token[8][1] = U
            // i = 8 j = 20 k = 2  string[20] → token[8][2] = A
            // i = 8 j = 21 k = 3  string[21] → token[8][3] = L

            // i = 9 j = 21 k = 0  string[22] → token[9][0] = ;

            token[i][k] = string[j];
            k++;
        }
    }
    printf("outside loop\n");

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            printf("At index i=%d j=%d %c\n", i, j, token[i][j]);
        }
    }

    return 0;
}
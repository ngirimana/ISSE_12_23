/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char *argv[])
{
    char *input = NULL;
    printf("Welcome to Plaid Shell!\n");

    while (1)
    {
        input = readline("\n#? ");

        if (input == NULL || strcasecmp(input, "quit") == 0 || strcasecmp(input, "exit") == 0)
        {
            exit(1);
        }
        if (input && *input)
        {
            add_history(input);               // Adding input to history
            printf("You typed: %s\n", input); // Printing user input
        }

        free(input);
    }

    return 0;
}

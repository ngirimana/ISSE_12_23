/*
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    char *input;

    while (1)
    {
        input = readline("> "); // Displaying '> ' as a prompt

        if (!input)
        {
            printf("Exiting...\n");
            break;
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

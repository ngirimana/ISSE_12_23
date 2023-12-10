#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "clist.h"
#include "tokenize.h"
#include "parse.h"
#include "pipeline.h"

// Function to remove backslashes from a string
void remove_backslashes(char *str)
{
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '\\')
        {
            str[j++] = str[i];
        }
        else if (str[i + 1] == '\\')
        {
            str[j++] = '\\';
            i++;
        }
    }
    str[j] = '\0';
}

int main(int argc, char *argv[])
{
    char *input = NULL;
    char errmsg[128];
    printf("Welcome to Plaid Shell!\n");

    while (1)
    {
        input = readline("#? ");

        if (input == NULL || strcasecmp(input, "quit") == 0 || strcasecmp(input, "exit") == 0)
        {
            exit(1);
        }
        if (input && *input)
        {
            add_history(input); // Adding input to history

            CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));

            Pipeline pipeline = Parse(tokens, errmsg, sizeof(errmsg));

            char *pipeline_string = GetPipelineString(pipeline);

            if (strncmp(pipeline_string, "cd ", 3) == 0)
            {
                // If the input starts with 'cd'
                char *directory = pipeline_string + 3; // Extract directory path
                if (strcmp(directory, "~") == 0)
                {
                    char *home_dir = getenv("HOME");
                    if (home_dir != NULL)
                    {
                        int change_dir_status = chdir(home_dir);
                        if (change_dir_status != 0)
                        {
                            perror("chdir");
                        }
                    }
                    else
                    {
                        printf("HOME environment variable not set.\n");
                    }
                }
                else
                {
                    remove_backslashes(directory); // Remove backslashes

                    int change_dir_status = chdir(directory);
                    if (change_dir_status != 0)
                    {
                        perror("chdir");
                    }
                }
            }
            else
            {
                pid_t pid = fork();

                if (pid < 0)
                {
                    perror("fork");
                    exit(1);
                }
                else if (pid == 0)
                {
                    // Child process
                    execl("/bin/sh", "sh", "-c", pipeline_string, NULL);
                    perror("execl");
                    exit(1);
                }
                else
                {
                    // Parent process
                    int status;
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                    {
                        WEXITSTATUS(status);
                    }
                }
            }
        }

        free(input);
    }

    return 0;
}

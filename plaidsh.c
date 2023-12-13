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

void compareStrings(char *str1, char *str2)
{
    int i;
    for (i = 0; str1[i] != '\0' && str2[i] != '\0'; i++)
    {
        if (str1[i] != str2[i])
        {
            printf("Difference found at index %d: '%c' in the first string and '%c' in the second string\n", i, str1[i], str2[i]);
            return;
        }
    }

    if (str1[i] == '\0' && str2[i] == '\0')
    {
        printf("Strings are identical\n");
    }
    else
    {
        printf("Difference found at index %d: End of string reached for one of the strings\n", i);
    }
}

void removeLastCharacter(char *str)
{
    if (str == NULL || str[0] == '\0')
    {
        return; // Handle empty string or NULL pointer
    }

    int len = strlen(str);
    if (len > 0)
    {
        str[len - 1] = '\0';
    }
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
            exit(0);
        }
        if (input && *input)
        {
            add_history(input); // Adding input to history

            CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));

            Pipeline pipeline = Parse(tokens, errmsg, sizeof(errmsg));

            char *pipeline_string = PL_GetPipelineString(pipeline);

            if (strncmp(pipeline_string, "cd ", 3) == 0)
            {
                // If the input starts with 'cd'
                char *directory = pipeline_string + 3; // Extract directory path
                if (strcmp(directory, "~") == 0 || strcmp(pipeline_string, "cd ") == 0)
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
                    remove_backslashes(directory);  // Remove backslashes
                    removeLastCharacter(directory); // Remove trailing newline character
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

                    if (strncmp(pipeline_string, "author", 6) == 0)
                    {
                        char *args = pipeline_string + 6;

                        if (strlen(pipeline_string) > 6)
                        {
                            char *command = malloc(strlen("echo NGIRIMANA Schadrack") + strlen(args) + 1);
                            strcpy(command, "echo NGIRIMANA Schadrack");
                            strcat(command, args);
                            execl("/bin/bash", "bash", "-c", command, NULL);
                            free(command);
                        }
                        else
                        {
                            char *args1[] = {"bash", "-c", "echo $author", NULL};
                            char *env_args[] = {"author=NGIRIMANA Schadrack", NULL}; // Set the environment variable
                            execle("/bin/bash", "bash", "-c", args1[2], NULL, env_args);
                        }
                    }
                    // Child process
                    execl("/bin/sh", "sh", "-c", pipeline_string, NULL);

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
                        int exit_status = WEXITSTATUS(status);
                        if (exit_status != 0)
                        {
                            char *token = strtok(pipeline_string, " ");
                            printf("%s: Command not found.*Child.*exited with status 2", token);
                        }
                    }
                }
            }
            free(pipeline_string);
            CL_free(tokens);
            PL_Free(pipeline);
        }

        free(input);
    }

    return 0;
}

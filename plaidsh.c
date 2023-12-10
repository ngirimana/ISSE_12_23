/*
 *
 */

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
            printf("Pipeline: %s\n", pipeline_string);

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
                    printf("Child process exited with status %d\n", WEXITSTATUS(status));
                }
            }
        }

        free(input);
    }

    return 0;
}

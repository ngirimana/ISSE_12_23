#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parse.h"
#include "pipeline.h"
#include "tokenize.h"
#include "clist.h"
#include "token.h"

Pipeline Parse(CList tokens, char *errmsg, size_t errmsg_sz)
{
    Pipeline new_pipeline = PL_New();

    if (tokens != NULL)
    {
        printf("Parsing tokens...\n");
        Token first_token = TOK_next(tokens);

        if (TOK_next_type(tokens) == TOK_WORD)
        {
            Command new_command = PL_InitCommand(first_token.value);

            TOK_consume(tokens);
            while (TOK_next_type(tokens) != TOK_END)
            {
                printf("Inside loop.\n");
                Token next_token = TOK_next(tokens);
                if (next_token.type == TOK_WORD)
                {
                    new_command = PL_AddArgument(new_command, next_token.value);
                    TOK_consume(tokens);
                }
                else if (next_token.type == TOK_QUOTED_WORD)
                {
                    char *quoted_word = (char *)malloc(strlen(next_token.value) + 3);
                    if (quoted_word != NULL)
                    {
                        strcpy(quoted_word, "\"");
                        strcat(quoted_word, next_token.value);
                        strcat(quoted_word, "\"");
                        new_command = PL_AddArgument(new_command, quoted_word);
                        free(quoted_word); // Free dynamically allocated memory
                    }
                    TOK_consume(tokens);
                }

                else if (next_token.type == TOK_PIPE)
                {
                    TOK_consume(tokens); // Consume the pipe token

                    if (TOK_next_type(tokens) == TOK_WORD)
                    {
                        new_pipeline = PL_AddCommand(new_pipeline, new_command);
                        printf("Found PIPE.\n");

                        new_command = PL_InitCommand(TOK_next(tokens).value);
                        printf("pipe#################%s", GetPipelineString(new_pipeline));
                        TOK_consume(tokens); // Consume the command name token
                    }
                    else
                    {
                        printf("Error: Expected command after pipe.\n");
                        // Handle the error condition
                    }
                }
            }

            // Add the last constructed command to the pipeline outside the loop
            new_pipeline = PL_AddCommand(new_pipeline, new_command);

            printf("%d\n", PipelineLength(new_pipeline));
            return new_pipeline;
        }
        else
        {
            sprintf(errmsg, "Invalid start token: %s!, Linux command always start by word ", CL_nth(tokens, 0).value);
            printf("Error: %s\n", errmsg);
        }
    }
    return new_pipeline;
}

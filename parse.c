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
        Token first_token = TOK_next(tokens);

        if (first_token.type == TOK_WORD)
        {
            Command new_command = PL_InitCommand(first_token.value);
            TOK_consume(tokens);

            while (TOK_next_type(tokens) != TOK_END)
            {
                Token next_token = TOK_next(tokens);

                if (next_token.type == TOK_WORD || next_token.type == TOK_QUOTED_WORD)
                {
                    if (next_token.type == TOK_QUOTED_WORD)
                    {
                        // Handle quoted word
                        char *quoted_word = (char *)malloc(strlen(next_token.value) + 3);
                        if (quoted_word != NULL)
                        {
                            strcpy(quoted_word, "\"");
                            strcat(quoted_word, next_token.value);
                            strcat(quoted_word, "\"");
                            new_command = PL_AddArgument(new_command, quoted_word);
                            free(quoted_word);
                        }
                    }
                    else
                    {
                        // Handle regular word
                        new_command = PL_AddArgument(new_command, next_token.value);
                    }
                    TOK_consume(tokens);
                }
                else if (next_token.type == TOK_LESSTHAN || next_token.type == TOK_GREATERTHAN)
                {
                    printf("Redirect\n");
                    // Handle redirection
                    char *redirect_symbol = (next_token.type == TOK_LESSTHAN) ? "<" : ">";
                    TOK_consume(tokens);

                    if (TOK_next_type(tokens) == TOK_WORD || TOK_next_type(tokens) == TOK_QUOTED_WORD)
                    {
                        char *file_name = (char *)malloc(strlen(TOK_next(tokens).value) + 3);
                        if (file_name != NULL)
                        {
                            strcpy(file_name, redirect_symbol);

                            if (TOK_next_type(tokens) == TOK_QUOTED_WORD)
                            {
                                strcat(file_name, "\"");
                            }

                            strcat(file_name, TOK_next(tokens).value);

                            if (TOK_next_type(tokens) == TOK_QUOTED_WORD)
                            {
                                strcat(file_name, "\"");
                            }

                            new_command = PL_AddArgument(new_command, file_name);

                            free(file_name);
                        }
                        TOK_consume(tokens);
                    }
                    else
                    {
                        printf("Expect filename after redirection\n");
                        // Handle the error condition or exit the loop
                        // For instance, break; or return an error code
                        break;
                    }
                }

                else if (next_token.type == TOK_PIPE)
                {
                    // Handle pipe
                    TOK_consume(tokens);
                    if (TOK_next_type(tokens) == TOK_PIPE)
                    {
                        printf("No command specified\n");
                    }
                    else if (TOK_next_type(tokens) == TOK_WORD)
                    {
                        new_pipeline = PL_AddCommand(new_pipeline, new_command);
                        new_command = PL_InitCommand(TOK_next(tokens).value);
                        TOK_consume(tokens);
                    }
                    else
                    {
                        printf("No command specified\n");
                        // Handle the error condition
                    }
                }
                else
                {
                    // Handle unrecognized token type
                    printf("Error: Unrecognized token type.\n");
                    break; // Break the loop in case of unrecognized token type
                }
            }

            // Add the last constructed command to the pipeline outside the loop
            new_pipeline = PL_AddCommand(new_pipeline, new_command);

            printf("%d\n", PipelineLength(new_pipeline));
            return new_pipeline;
        }
        else
        {
            printf("No command specified\n");
        }
    }
    return new_pipeline;
}

/*
 * tokenize.c
 *
 * Functions to tokenize and manipulate lists of tokens
 *
 * Author: Ngirimana Schadrack <schadran@andrew.cmu.edu>
 */
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "clist.h"
#include "tokenize.h"
#include "token.h"

#define MAX_TOKEN_LENGTH 1024

// Documented in .h file
const char *TT_to_str(TokenType tt)
{
    switch (tt)
    {
    case TOK_WORD:
        return "WORD";
    case TOK_QUOTED_WORD:
        return "QUOTED WORD";
    case TOK_GREATERTHAN:
        return ">";
    case TOK_LESSTHAN:
        return "<";
    case TOK_PIPE:
        return "|";
    case TOK_END:
        return "(end)";
    }
    __builtin_unreachable();
}

/*
 * Helper function to determine the type of a token
 *
 * Parameters:
 *   c   The character to check
 *
 * Returns: the type of the token
 */

void printToken(TokenType type, const char *value)
{
    switch (type)
    {
    case TOK_WORD:
        printf("WORD \"%s\"; ", value);
        break;
    case TOK_QUOTED_WORD:
        printf("QUOTED_WORD \"%s\"; ", value);
        break;
    case TOK_LESSTHAN:
        printf("LESSTHAN; ");
        break;
    case TOK_GREATERTHAN:
        printf("GREATERTHAN; ");
        break;
    case TOK_PIPE:
        printf("PIPE; ");
        break;
    default:
        break;
    }
}

// Helper function to determine the type of a token
TokenType determineTokenType(char c)
{
    switch (c)
    {
    case '|':
        return TOK_PIPE;
    case '<':
        return TOK_LESSTHAN;
    case '>':
        return TOK_GREATERTHAN;
    }
    return TOK_WORD; // If not a special character, default to WORD
}

// Documented in .h file
CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz)
{
    CList tokens = CL_new();
    char buffer[MAX_TOKEN_LENGTH];
    int buffer_index = 0;
    int input_length = strlen(input);
    int in_quote = 0;
    int escaped = 0;

    for (int i = 0; i < input_length; ++i)
    {
        char current_char = input[i];
        Token token;

        if (current_char == '\\' && !escaped)
        {
            escaped = 1;
            continue; // Skip the current character
        }

        if (escaped)
        {
            escaped = 0;
            switch (current_char)
            {
            case 'n':
                current_char = '\n';
                break;
            case 'r':
                current_char = '\r';
                break;
            case 't':
                current_char = '\t';
                break;

            default:

                buffer[buffer_index++] = '\\'; // Add the escaped character back
                if (current_char != '<' && current_char != '>' && current_char != '|')
                {
                    printf("Illegal escape character %c\n", current_char);
                }
                break;
            }
        }
        else if (current_char == '"')
        {
            in_quote = !in_quote;
            if (in_quote)
            {
                if (buffer_index > 0)
                {
                    buffer[buffer_index] = '\0';
                    token.type = TOK_WORD;
                    strncpy(token.value, buffer, MAX_TOKEN_LENGTH);
                    CL_append(tokens, token);
                    buffer_index = 0;
                }
            }
            else
            {
                if (buffer_index > 0)
                {
                    buffer[buffer_index] = '\0';
                    token.type = TOK_QUOTED_WORD;
                    strncpy(token.value, buffer, MAX_TOKEN_LENGTH);
                    CL_append(tokens, token);
                    buffer_index = 0;
                }
            }
            continue;
        }
        else if (!in_quote)
        {
            TokenType special_type = determineTokenType(current_char);
            if (special_type != TOK_WORD)
            {
                if (buffer_index > 0)
                {
                    buffer[buffer_index] = '\0';
                    token.type = TOK_WORD;
                    strncpy(token.value, buffer, MAX_TOKEN_LENGTH);
                    CL_append(tokens, token);
                    buffer_index = 0;
                }
                token.type = special_type;

                token.value[0] = current_char;
                token.value[1] = '\0'; // Null terminate the string

                CL_append(tokens, token);
                continue;
            }
            else if (isspace(current_char))
            {
                if (buffer_index > 0)
                {
                    buffer[buffer_index] = '\0';
                    token.type = TOK_WORD;
                    strncpy(token.value, buffer, MAX_TOKEN_LENGTH);
                    CL_append(tokens, token);
                    buffer_index = 0;
                }
                continue;
            }
        }

        buffer[buffer_index++] = current_char;
    }

    if (buffer_index > 0)
    {
        Token token;
        buffer[buffer_index] = '\0';
        if (in_quote)
        {
            printf("Unterminated quote\n");
            token.type = TOK_END;
            
        }
        else
        {
            token.type = TOK_WORD;
            strncpy(token.value, buffer, MAX_TOKEN_LENGTH);
        }
        CL_append(tokens, token);
    }

    return tokens;
}

// Documented in .h file
TokenType TOK_next_type(CList tokens)
{

    if (CL_length(tokens) > 0)
    {
        Token token = CL_nth(tokens, 0);
        return token.type;
    }
    else
        return TOK_END;
}

// Documented in .h file
Token TOK_next(CList tokens)
{
    Token token;
    if (CL_length(tokens) > 0)
    {
        token = CL_nth(tokens, 0);
    }
    else
    {
        token.type = TOK_END;
    }
    return token;
}

// Documented in .h file
void TOK_consume(CList tokens)
{
    if (tokens == NULL)
        return;

    if (tokens->head != NULL)
        CL_remove(tokens, 0);
}

void TOK_print(CList tokens)
{
    Token token;

    while (CL_length(tokens) > 0)
    {
        token = CL_nth(tokens, 0);

        switch (token.type)
        {
        case TOK_WORD:
            if (CL_length(tokens) == 1)
                printf("WORD “%s”", token.value);
            else
                printf("WORD “%s”; ", token.value);
            break;
        case TOK_QUOTED_WORD:
            if (CL_length(tokens) == 1)
                printf("QUOTED_WORD “%s”", token.value);
            else
                printf("QUOTED_WORD “%s”; ", token.value);
            break;
        case TOK_LESSTHAN:
            if (CL_length(tokens) == 1)
                printf("LESSTHAN");
            else
                printf("LESSTHAN;");
            break;
        case TOK_GREATERTHAN:
            if (CL_length(tokens) == 1)
                printf("GREATERTHAN");
            else
                printf("GREATERTHAN;");
            break;
        case TOK_PIPE:
            if (CL_length(tokens) == 1)
                printf("PIPE");
            else
                printf("PIPE;");
            break;
        default:
            break;
        }

        CL_remove(tokens, 0);
    }

    printf("\n");
}

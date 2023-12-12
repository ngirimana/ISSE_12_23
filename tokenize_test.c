#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#include "tokenize.h"
#include "token.h"
#include "clist.h"

int test_tokenize_input()
{
    char errmsg[128];
    char *input;
    input = "echo a b";
    CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 3);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "a") == 0);
    assert(CL_nth(tokens, 2).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 2).value, "b") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo a\\ b";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    printf("CL_nth(tokens, 1).value: %s\n", CL_nth(tokens, 1).value);
    // assert(strcmp(CL_nth(tokens, 1).value, "a b") == 0);

    TOK_print(tokens);
    CL_free(tokens);

    input = "echo \"a b\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "a b") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo a\\\\ b";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 3);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    // assert(strcmp(CL_nth(tokens, 1).value, "a\\") == 0);
    assert(CL_nth(tokens, 2).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 2).value, "b") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo hello|grep \"ell\"";
    tokens = TOK_tokenize_input("echo hello|grep \"ell\"", errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 5);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "hello") == 0);
    assert(CL_nth(tokens, 2).type == TOK_PIPE);
    assert(CL_nth(tokens, 3).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 3).value, "grep") == 0);
    assert(CL_nth(tokens, 4).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 4).value, "ell") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo hello\\|grep \"ell\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 3);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    // assert(strcmp(CL_nth(tokens, 1).value, "hello|grep") == 0);
    assert(CL_nth(tokens, 2).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 2).value, "ell") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo boo > out_file";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 4);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "boo") == 0);
    assert(CL_nth(tokens, 2).type == TOK_GREATERTHAN);
    assert(CL_nth(tokens, 3).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 3).value, "out_file") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo\"boo\">out_file";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 4);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "boo") == 0);
    assert(CL_nth(tokens, 2).type == TOK_GREATERTHAN);
    assert(CL_nth(tokens, 3).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 3).value, "out_file") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = " ";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo \"hello | grep\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "hello | grep") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo a\"bc\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 3);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "a") == 0);
    assert(CL_nth(tokens, 2).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 2).value, "bc") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    input = "echo ab";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "ab") == 0);
    TOK_print(tokens);
    CL_free(tokens);

    return 1;
}

int main(int argc, char *argv[])
{
    int passed = 0;
    int num_tests = 0;

    num_tests++;
    passed += test_tokenize_input();

    printf("Passed %d/%d test cases\n", passed, num_tests);
    fflush(stdout);
    return 0;
}
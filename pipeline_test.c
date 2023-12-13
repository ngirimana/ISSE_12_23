#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> // strlen
#include "pipeline.h"
#include "tokenize.h"
#include "token.h"
#include "clist.h"

#define test_assert(value)                                               \
    {                                                                    \
        if (!(value))                                                    \
        {                                                                \
            printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value); \
            return 0;                                                    \
        }                                                                \
    }

// Function to compare two strings
int compare_strings(const char *str1, const char *str2)
{
    if (strcmp(str1, str2) == 0)
    {
        return 1;
    }
    else
    {
        exit(1);
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
int testing_tokenize_usage()
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

    CL_free(tokens);

    input = "echo \"a b\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "a b") == 0);
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
    CL_free(tokens);

    input = "echo hello\\|grep \"ell\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 3);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(CL_nth(tokens, 2).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 2).value, "ell") == 0);
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
    CL_free(tokens);

    input = " ";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 0);
    CL_free(tokens);

    input = "echo \"hello | grep\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_QUOTED_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "hello | grep") == 0);
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
    CL_free(tokens);

    input = "echo ab";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    assert(tokens != NULL);
    assert(CL_length(tokens) == 2);
    assert(CL_nth(tokens, 0).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 0).value, "echo") == 0);
    assert(CL_nth(tokens, 1).type == TOK_WORD);
    assert(strcmp(CL_nth(tokens, 1).value, "ab") == 0);
    CL_free(tokens);
    return 1;
}

int testing_parse_usage()
{
    return 1;
}

int test_commands_usage()
{
    Command command = PL_InitCommand("ls");
    PL_AddArgument(command, "-l");
    PL_AddArgument(command, "-a");
    PL_AddArgument(command, "-h");
    char *command_string = PL_GetCommandString(command);

    removeLastCharacter(command_string);
    test_assert(compare_strings(command_string, "ls -l -a -h"));

    free(command_string);
    PL_CommandFree(command);

    Command com1 = PL_InitCommand("cat");
    PL_AddArgument(com1, "file.txt");
    char *command_string1 = PL_GetCommandString(com1);
    removeLastCharacter(command_string1);
    test_assert(compare_strings(command_string1, "cat file.txt"));
    free(command_string1);
    PL_CommandFree(com1);

    Command com2 = PL_InitCommand("grep");
    PL_AddArgument(com2, "hello");
    PL_AddArgument(com2, "file.txt");
    char *command_string2 = PL_GetCommandString(com2);
    removeLastCharacter(command_string2);
    test_assert(compare_strings(command_string2, "grep hello file.txt"));
    free(command_string2);
    PL_CommandFree(com2);

    Command com3 = PL_InitCommand("ls");
    com3 = PL_AddArgument(com3, "-l");
    com3 = PL_AddArgument(com3, "|");
    com3 = PL_AddArgument(com3, "grep");
    com3 = PL_AddArgument(com3, "hello");
    com3 = PL_AddArgument(com3, "|");
    com3 = PL_AddArgument(com3, "wc");
    char *command_string3 = PL_GetCommandString(com3);
    removeLastCharacter(command_string3);
    test_assert(compare_strings(command_string3, "ls -l | grep hello | wc"));
    free(command_string3);
    PL_CommandFree(com3);

    Command com4 = PL_InitCommand("ls");
    com4 = PL_AddArgument(com4, "-las");
    char *command_string4 = PL_GetCommandString(com4);
    removeLastCharacter(command_string4);
    test_assert(compare_strings(command_string4, "ls -las"));
    free(command_string4);
    PL_CommandFree(com4);

    return 1;
}

int test_pipeline_usage()
{
    Command command = PL_InitCommand("ls");
    PL_AddArgument(command, "-la");
    Pipeline newPipeline = PL_New();
    newPipeline = PL_AddCommand(newPipeline, command);
    Command command1 = PL_InitCommand("grep");
    PL_AddArgument(command1, "hello");
    newPipeline = PL_AddCommand(newPipeline, command1);
    char *pipeline_string = PL_GetPipelineString(newPipeline);
    removeLastCharacter(pipeline_string);

    test_assert(compare_strings(pipeline_string, "ls -la |grep hello"));

    test_assert(PL_Length(newPipeline) == 2);
    free(pipeline_string);
    PL_Free(newPipeline);
    return 1;
}

int main()
{
    int passed = 0;
    int num_tests = 0;
    num_tests += 1;
    passed += testing_tokenize_usage();
    num_tests += 1;
    passed += testing_parse_usage();
    num_tests += 1;
    passed += test_commands_usage();
    num_tests += 1;
    passed += test_pipeline_usage();

    printf("Passed %d/%d test cases\n", passed, num_tests);
    fflush(stdout);
    return 0;
}
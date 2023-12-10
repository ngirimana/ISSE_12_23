#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> // strlen
#include "pipeline.h"

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

int test_commands()
{
    Command command = PL_InitCommand("ls");
    command = PL_AddArgument(command, "-l");
    command = PL_AddArgument(command, "-a");
    command = PL_AddArgument(command, "-h");
    char *command_string = GetCommandString(command);

    test_assert(compare_strings(command_string, "ls -l -a -h"));
    free(command_string);
    CommandFree(command);

    Command com1 = PL_InitCommand("cat");
    com1 = PL_AddArgument(com1, "file.txt");
    char *command_string1 = GetCommandString(com1);
    test_assert(compare_strings(command_string1, "cat file.txt"));

    free(command_string1);
    CommandFree(com1);

    Command com2 = PL_InitCommand("grep");
    com2 = PL_AddArgument(com2, "hello");
    com2 = PL_AddArgument(com2, "file.txt");
    char *command_string2 = GetCommandString(com2);
    test_assert(compare_strings(command_string2, "grep hello file.txt"));

    free(command_string2);
    CommandFree(com2);

    Command com3 = PL_InitCommand("ls");
    com3 = PL_AddArgument(com3, "-l");
    com3 = PL_AddArgument(com3, "|");
    com3 = PL_AddArgument(com3, "grep");
    com3 = PL_AddArgument(com3, "hello");
    com3 = PL_AddArgument(com3, "|");
    com3 = PL_AddArgument(com3, "wc");
    char *command_string3 = GetCommandString(com3);
    test_assert(compare_strings(command_string3, "ls -l | grep hello | wc"));

    free(command_string3);
    CommandFree(com3);

    Command com4 = PL_InitCommand("ls");
    com4 = PL_AddArgument(com4, "-las");
    char *command_string4 = GetCommandString(com4);
    test_assert(compare_strings(command_string4, "ls -las"));

    free(command_string4);
    CommandFree(com4);

    return 1;
}

int test_pipeline()
{
    Command command = PL_InitCommand("ls");
    command = PL_AddArgument(command, "-la");
    Pipeline newPipeline = PL_New();
    newPipeline = PL_AddCommand(newPipeline, command);
    Command command1 = PL_InitCommand("grep");
    command1 = PL_AddArgument(command1, "hello");
    newPipeline = PL_AddCommand(newPipeline, command1);
    char *pipeline_string = GetPipelineString(newPipeline);
    printf("%s\n", pipeline_string);
    test_assert(compare_strings(pipeline_string, "ls -la|grep hello"));

    test_assert(PipelineLength(newPipeline) == 2);
    free(pipeline_string);

    PipelineFree(newPipeline);
    return 1;
}

int main()
{
    int passed = 0;
    int num_tests = 0;
    num_tests += 1;
    passed += test_commands();
    num_tests += 1;
    passed += test_pipeline();

    printf("Passed %d/%d test cases\n", passed, num_tests);
    fflush(stdout);
    return 0;
}
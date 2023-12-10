#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pipeline.h"

struct _command
{
    char command[MAX_FILENAME_LENGTH];
    char *arguments[MAX_ARGS];
    int num_arguments;
};

struct _pipeline
{
    Command commands[MAX_COMMANDS];
    char input_file[MAX_FILENAME_LENGTH];
    char output_file[MAX_FILENAME_LENGTH];
    int num_commands;
};

Pipeline PL_New()
{
    Pipeline new_pipeline = (Pipeline)malloc(sizeof(struct _pipeline));
    if (new_pipeline != NULL)
    {
        new_pipeline->num_commands = 0;
        for (int i = 0; i < MAX_COMMANDS; ++i)
        {
            memset(&new_pipeline->commands[i], 0, sizeof(struct _command));
        }
        new_pipeline->input_file[0] = '\0';
        new_pipeline->output_file[0] = '\0';
    }
    return new_pipeline;
}

Command PL_New_Command()
{
    Command new_command = (Command)malloc(sizeof(struct _command));
    assert(new_command);

    new_command->num_arguments = 0;
    for (int i = 0; i < MAX_ARGS; ++i)
    {
        new_command->arguments[i] = NULL;
    }

    return new_command;
}

Command PL_InitCommand(const char *executable)
{
    Command new_command = PL_New_Command();
    if (new_command != NULL && executable != NULL)
    {
        strncpy(new_command->command, executable, MAX_FILENAME_LENGTH - 1);
        new_command->command[MAX_FILENAME_LENGTH - 1] = '\0'; // Ensure null-terminated string
    }
    return new_command;
}

Command PL_AddArgument(Command command, const char *argument)
{
    if (command == NULL || command->num_arguments >= MAX_ARGS || argument == NULL)
    {
        return command;
    }

    size_t arg_len = strlen(argument);
    command->arguments[command->num_arguments] = (char *)malloc(arg_len + 1);
    if (command->arguments[command->num_arguments] != NULL)
    {
        strncpy(command->arguments[command->num_arguments], argument, arg_len);
        command->arguments[command->num_arguments][arg_len] = '\0';
        command->num_arguments++;
    }
    return command;
}

Pipeline PL_AddCommand(Pipeline pipeline, Command command)
{
    if (command == NULL || pipeline->num_commands >= MAX_COMMANDS)
    {
        return pipeline;
    }

    pipeline->commands[pipeline->num_commands++] = command;
    return pipeline;
}

void SetInputFile(Pipeline pipeline, const char *filename)
{
    if (pipeline != NULL && filename != NULL)
    {
        strncpy(pipeline->input_file, filename, MAX_FILENAME_LENGTH - 1);
        pipeline->input_file[MAX_FILENAME_LENGTH - 1] = '\0'; // Ensure null-terminated string
    }
}

void SetOutputFile(Pipeline pipeline, const char *filename)
{
    if (pipeline != NULL && filename != NULL)
    {
        strncpy(pipeline->output_file, filename, MAX_FILENAME_LENGTH - 1);
        pipeline->output_file[MAX_FILENAME_LENGTH - 1] = '\0'; // Ensure null-terminated string
    }
}
char *GetInputFile(Pipeline pipeline)
{
    if (pipeline != NULL)
    {
        return pipeline->input_file;
    }
    return NULL;
}

char *GetOutputFile(Pipeline pipeline)
{
    if (pipeline != NULL)
    {
        return pipeline->output_file;
    }
    return NULL;
}

char *GetCommandString(Command command)
{
    if (command != NULL)
    {
        char *command_string = (char *)malloc(MAX_FILENAME_LENGTH + 1);
        if (command_string != NULL)
        {
            strncpy(command_string, command->command, MAX_FILENAME_LENGTH - 1);
            strncat(command_string, " ", MAX_FILENAME_LENGTH - 1);
            for (int i = 0; i < command->num_arguments; ++i)
            {

                strncat(command_string, command->arguments[i], MAX_FILENAME_LENGTH - strlen(command_string) - 1);
                if (i < command->num_arguments - 1)
                {
                    strcat(command_string, " "); // Add a space if it's not the last argument
                }
            }
        }
        return command_string;
    }
    return NULL;
}

char *GetPipelineString(Pipeline pipeline)
{
    if (pipeline != NULL)
    {
        char *pipeline_string = (char *)malloc(MAX_FILENAME_LENGTH + 1);
        if (pipeline_string != NULL)
        {
            pipeline_string[0] = '\0';
            for (int i = 0; i < pipeline->num_commands; ++i)
            {
                char *command_string = GetCommandString(pipeline->commands[i]);
                strncat(pipeline_string, command_string, MAX_FILENAME_LENGTH - strlen(pipeline_string) - 1);
                free(command_string);
                if (i < pipeline->num_commands - 1)
                {
                    strcat(pipeline_string, "|"); // Add a space if it's not the last argument
                }
            }
        }
        return pipeline_string;
    }
    return NULL;
}

void CommandFree(Command command)
{
    if (command != NULL)
    {
        for (int i = 0; i < command->num_arguments; ++i)
        {
            free(command->arguments[i]);
        }
        free(command);
    }
}

void PipelineFree(Pipeline pipeline)
{
    if (pipeline != NULL)
    {
        for (int i = 0; i < pipeline->num_commands; ++i)
        {
            CommandFree(pipeline->commands[i]);
        }
        free(pipeline);
    }
}

int PipelineLength(Pipeline pipeline)
{
    if (pipeline != NULL)
    {
        return pipeline->num_commands;
    }
    return 0;
}

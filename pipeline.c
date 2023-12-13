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

typedef struct _command_node
{
    struct _command_node *next;
    struct _command data;
} CommandNode;

typedef CommandNode *CommandList;

struct _pipeline
{
    CommandList head; // Renamed from commands to head for the linked list of commands
    char input_file[MAX_FILENAME_LENGTH];
    char output_file[MAX_FILENAME_LENGTH];
    int num_commands;
};

// typedef struct _pipeline *Pipeline;

Pipeline PL_New()
{
    Pipeline new_pipeline = (Pipeline)malloc(sizeof(struct _pipeline));
    if (new_pipeline != NULL)
    {
        new_pipeline->num_commands = 0;
        new_pipeline->head = NULL; // Initialize head as an empty linked list
        new_pipeline->input_file[0] = '\0';
        new_pipeline->output_file[0] = '\0';
    }
    return new_pipeline;
}

Command PL_InitCommand(const char *executable)
{
    Command new_command = (Command)malloc(sizeof(struct _command));
    // assert(new_command);

    new_command->num_arguments = 0;
    for (int i = 0; i < MAX_ARGS; ++i)
    {
        new_command->arguments[i] = NULL;
    }

    if (executable != NULL)
    {
        strncpy(new_command->command, executable, MAX_FILENAME_LENGTH - 1);
        new_command->command[MAX_FILENAME_LENGTH - 1] = '\0'; // Ensure null-terminated string
    }

    return new_command;
}

Command PL_AddArgument(Command command, const char *argument)
{
    if (command == NULL || argument == NULL)
    {
        return command;
    }

    size_t arg_len = strlen(argument);
    char *new_arg = (char *)malloc(arg_len + 1);
    if (new_arg != NULL)
    {
        strncpy(new_arg, argument, arg_len);
        new_arg[arg_len] = '\0';
        command->arguments[command->num_arguments++] = new_arg;
    }
    // free(new_arg);
    return command;
}

Pipeline PL_AddCommand(Pipeline pipeline, Command command)
{
    if (command == NULL || pipeline == NULL)
    {
        return pipeline;
    }

    CommandNode *new_node = (CommandNode *)malloc(sizeof(CommandNode));
    if (new_node != NULL)
    {
        new_node->data = *command;
        new_node->next = NULL; // Set the 'next' pointer to NULL for the new node
        if (pipeline->head == NULL)
        {
            pipeline->head = new_node;
        }
        else
        {
            CommandNode *temp = pipeline->head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = new_node;
        }

        pipeline->num_commands++;
    }
    free(command);

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
            command_string[MAX_FILENAME_LENGTH - 1] = '\0'; // Ensure null-terminated string
            strncat(command_string, " ", MAX_FILENAME_LENGTH - 1);
            for (int i = 0; i < command->num_arguments; ++i)
            {
                strncat(command_string, command->arguments[i], MAX_FILENAME_LENGTH - strlen(command_string) - 1);
                strncat(command_string, " ", MAX_FILENAME_LENGTH - strlen(command_string) - 1);
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
            CommandNode *current = pipeline->head;
            while (current != NULL)
            {
                char *command_string = GetCommandString(&current->data);
                strncat(pipeline_string, command_string, MAX_FILENAME_LENGTH - strlen(pipeline_string) - 1);
                free(command_string);
                current = current->next;
                if (current != NULL)
                {
                    strcat(pipeline_string, "|");
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
        CommandNode *current = pipeline->head;
        while (current != NULL)
        {
            CommandNode *temp = current;
            current = current->next;
            for (int i = 0; i < temp->data.num_arguments; ++i)
            {
                free(temp->data.arguments[i]);
            }
            free(temp);
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

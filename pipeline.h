#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <stdio.h>

#define MAX_ARGS 10
#define MAX_COMMANDS 10
#define MAX_FILENAME_LENGTH 1024

typedef struct _command *Command;
typedef struct _pipeline *Pipeline;

Pipeline PL_New();
Command PL_InitCommand(const char *executable);
Command PL_AddArgument(Command command, const char *argument);
Pipeline PL_AddCommand(Pipeline pipeline, Command command);
int PipelineLength(Pipeline pipeline);
void SetInputFile(Pipeline pipeline, const char *filename);
char *GetOutputFile(Pipeline pipeline);
void SetOutputFile(Pipeline pipeline, const char *filename);
char *GetInputFile(Pipeline pipeline);
char *GetCommandString(Command command);
char *GetPipelineString(Pipeline pipeline);
void CommandFree(Command command);
void PipelineFree(Pipeline pipeline);

#endif /* _PIPELINE_H_ */

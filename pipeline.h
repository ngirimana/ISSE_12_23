/*
 * pipeline.h
 *
 * Simple description
 *
 * Author: Ngirimana Schadrack <schadran@andrew.cmu.edu>
 */

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <stdio.h>

#define MAX_ARGS 100
#define MAX_COMMANDS 100
#define MAX_FILENAME_LENGTH 1024

typedef struct _command *Command;
typedef struct _pipeline *Pipeline;
/*
 * Create a new pipeline
 *
 * Parameters: None
 *
 * Returns: The new pipeline
 */

Pipeline PL_New();
/*
 * Initialize a new command
 *
 * Parameters:
 *   executable   The executable for the command
 *
 * Returns: The new command
 */

Command PL_InitCommand(const char *executable);
/*
 * Add an argument to a command
 *
 * Parameters:
 *   command   The command
 *   argument  The argument to add
 *
 * Returns: The command
 */
Command PL_AddArgument(Command command, const char *argument);
/*
 * Add a command to a pipeline
 *
 * Parameters:
 *   pipeline   The pipeline
 *   command    The command to add
 *
 * Returns: The pipeline
 */
Pipeline PL_AddCommand(Pipeline pipeline, Command command);
/*
 * Get Length of a pipeline
 *
 * Parameters:
 *  pipeline   The pipeline
 * Returns: The length of the pipeline
 */
int PL_Length(Pipeline pipeline);
/*
 * get command string
 *
 * Parameters:
 *  command   The command
 * Returns: The command string
 */

char *PL_GetCommandString(Command command);
/*
 * get pipeline string
 *
 * Parameters:
 *  pipeline   The pipeline
 * Returns: The pipeline string
 */
char *PL_GetPipelineString(Pipeline pipeline);
/*
 * Deallocation of a command
 *
 * Parameters:
 * command   The command
 * Returns: None
 */
void PL_CommandFree(Command command);
/*
 * Deallocation of a pipeline
 *
 * Parameters:
 * pipeline   The pipeline
 * Returns: None
 */
void PL_Free(Pipeline pipeline);

#endif /* _PIPELINE_H_ */

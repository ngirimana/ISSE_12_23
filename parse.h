/*
 * parse.h
 *
 * Simple description
 *
 * Author: Ngirimana Schadrack <schadran@andrew.cmu.edu>
 */

#ifndef _PARSE_H_
#define _PARSE_H_

#include "clist.h"
#include "pipeline.h"
/*
`* Parse a list of tokens into a pipeline
 *
 * Parameters:
 *   tokens   The list of tokens
 *   errmsg   Buffer to store an error message in case of error
 *   errmsg_sz  Size of the errmsg buffer
 *
 * Returns: The pipeline, or NULL in case of error
 */

Pipeline Parse(CList tokens, char *errmsg, size_t errmsg_sz);

#endif /* _PARSE_H_ */

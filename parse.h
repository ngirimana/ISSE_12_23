/*
 * parse.h
 *
 * Simple description
 *
 * Author: Howdy Pierce <howdy@sleepymoose.net>
 */

#ifndef _PARSE_H_
#define _PARSE_H_

#include "clist.h"
#include "pipeline.h"

Pipeline Parse(CList tokens, char *errmsg, size_t errmsg_sz);

#endif /* _PARSE_H_ */

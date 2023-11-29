/*
 * tokenize.h
 *
 * Functions to tokenize, and manipulate lists of tokens
 *
 * Author: Ngirimana Schadrack <schadran@andrew.cmu.edu>
 */

#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_

#include <math.h>
#include <stddef.h>
#include "plist.h"
#include "token.h"

/*
 * For diagnostics; convert a TokenType to a printable string
 *
 * Parameters:
 *   tt       The TokenType
 *
 * Returns: A string representing the TokenType
 */
const char *TT_to_str(TokenType tt);

/*
 * Tokenize a string entered by the user
 *
 * Parameters:
 *   input      The input as entered by the user
 *   errmsg     Return space for an error message, filled in in case of error
 *   errmsg_sz  The size of errmsg
 *
 * Returns: A newly-created CList representing the tokenized input,
 *   with one token per list element. If an error is encountered,
 *   copies an error message into errmsg and returns NULL.
 *
 *   It is up to the caller to call CL_free on the returned list.
 */
CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz);

/*
 * Returns the TokenType for the next token. Does not modify the list
 * of tokens.
 *
 * Parameters:
 *   tokens    The list of tokens
 *
 * Returns: The TokenType for the next token, or TOK_END if the list
 *   is empty.
 */
TokenType TOK_next_type(CList tokens);

/*
 * Returns the next token. Does not modify the list of tokens.
 *
 * Parameters:
 *   tokens    The list of tokens
 *
 * Returns: The next token.
 */
Token TOK_next(CList tokens);

/*
 * Consumes (discards) the next token in the list
 *
 * Parameters:
 *   tokens    The list of tokens
 *
 * Returns: None
 */
void TOK_consume(CList tokens);

/*
 * For debugging: Prints the list of tokens, one per line
 *
 * Parameters:
 *   tokens    The list of tokens
 *
 * Returns: None
 */
void TOK_print(CList tokens);

#endif /* _TOKENIZE_H_ */

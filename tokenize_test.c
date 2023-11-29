#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tokenize.h"
#include "token.h"
#include "plist.h"

int main(int argc, char *argv[])
{
    char errmsg[128];
    char *input;
    input = "echo a b";
    CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("1: %s: ", input);
    TOK_print(tokens);
    input = "echo a\\ b";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("2: %s: ", input);
    TOK_print(tokens);
    input = "echo \" a b \"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("3: %s: ", input);
    TOK_print(tokens);
    input = "echo a\\\\ b";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("4: %s: ", input);
    TOK_print(tokens);
    input = "echo hello|grep \"ell\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("5: %s: ", input);
    TOK_print(tokens);
    input = "echo hello\\|grep \"ell\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("6: %s: ", input);
    TOK_print(tokens);
    input = "echo boo > out_file";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("7: %s: ", input);
    TOK_print(tokens);
    input = "echo\"boo\">out_file";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("8: %s: ", input);
    TOK_print(tokens);
    input = " ";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("9: %s: ", input);
    TOK_print(tokens);
    input = "echo \"hello | grep\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("10: %s: ", input);
    TOK_print(tokens);
    input = "echo a\"bc\"";
    tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
    printf("11: %s: ", input);
    TOK_print(tokens);
}
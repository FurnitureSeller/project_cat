#ifndef GREP_H
#define GREP_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_pattern(int argc,char **argv,int optind);
char *string_append_expr(char *string, size_t *size, char const *expr,size_t size_expr);


#endif
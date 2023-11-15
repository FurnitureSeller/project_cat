#ifndef GREP_H
#define GREP_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkPattern(int *argc, char **argv[],int optind);
char *Optargunion(char *OneDinstring, size_t *Msize, char const *target,size_t size_target);


#endif
#ifndef GREP_H
#define GREP_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *Optargunion(char *OneDinstring, size_t *Msize, char const *target,size_t size_target);
char *filePattern(char *fileofPattern, size_t *Fsize,char const *target);

#endif
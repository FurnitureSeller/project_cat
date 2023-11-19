#ifndef GREP_H
#define GREP_H
#define _GNU_SOURCE
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *Mpattern;  //подобие динамической строки
  size_t Msize;
  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
  bool printMatched;
  bool onlyline;
  bool nonstop;
  char *patternfile;
  size_t Fsize;
} AllFlags;

char *Optargunion(char *OneDinstring, size_t *Msize, char const *target,
                  size_t size_target);
char *filePattern(char *fileofPattern, size_t *Fsize, char const *target);

#endif
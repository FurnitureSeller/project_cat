#ifndef __SOMECODE__
#define __SOMECODE__

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  bool flag_b;
  bool flag_e;
  bool flag_v;
  bool flag_n;
  bool flag_s;
  bool flag_t;
} AllFlags;

AllFlags ReadFlags(int argc, char **argv);
void CatSetTable(const char *table[static 256])
void Flag_T(const char *table[static 256]);
void Flag_E(const char *table[static 256]);
void CatSet_V(const char *table[static 256])


#endif
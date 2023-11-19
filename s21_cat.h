#ifndef CAT_H
#define CAT_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CatSetTable(const char *table[static 256]);
void Flag_T(const char *table[static 256]);
void Flag_E(const char *table[static 256]);
void CatSet_V(const char *table[static 256]);

#endif
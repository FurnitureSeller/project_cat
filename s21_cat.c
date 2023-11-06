#include "s21_cat.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool flag_b;
  bool flag_e;
  bool flag_v;
  bool flag_n;
  bool flag_s;
  bool flag_t;
} AllFlags;

AllFlags ReadFlags(int argc,char **argv) {  
AllFlags SensorFlags = {0, 0, 0, 0, 0, 0};
  struct option longopts[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0}
      }; 
  char FlagNow = 0;                       
 
  while ((FlagNow = getopt_long(argc, argv, "bevEnstT", longopts, NULL)) != -1) {
    switch (FlagNow) {
    case 'b':
    SensorFlags.flag_b = 1;
    break; case 'e':
    SensorFlags.flag_e = 1;
    SensorFlags.flag_v = 1;
    break; case 'v':
    SensorFlags.flag_v = 1;
    break; case 'E':
    SensorFlags.flag_e = 1;
    break; case 'n':
    SensorFlags.flag_n = 1;
    break; case 's':
    SensorFlags.flag_s = 1;
    break; case 't':
    SensorFlags.flag_v = 1;
    SensorFlags.flag_v = 1;
    break; case 'T':
    SensorFlags.flag_t = 1;
    }
  }
  return SensorFlags;
}

void CatFile(FILE *file, AllFlags flags,const char *table[static 256])
{
  int c = 0;
  int lines = 1;
  int last;
  int first = 0;
  bool sq = false;
  if(flags.flag_b && flags.flag_n)
    flags.flag_b = false;

  while(fread(&c,1,1,file) > 0) { 
    if(last == '\n') {
      if(flags.flag_s && c == '\n') {
        if(sq)
          continue;
         sq = true;
      }
      else
          sq = false;
      if (flags.flag_b) {
        if(c != '\n')
            printf("%6i\t",lines++);
      }
      else if(flags.flag_n) {
          printf("%6i\t",lines++);
      }
    }
  if (first == 0) {
      if (flags.flag_b) {
        if(c != '\n')
            printf("%6i\t",lines++);
      }
      else if(flags.flag_n) {
          printf("%6i\t",lines++);
      }
    first++;
  }
  
  if(!*table[c])
    printf("%c",'\0');
  else
    printf("%s",table[c]);
  last = c;
  }
}

void Cat(int argc,char *argv[],AllFlags flags,const char *table[static 256]) {

  for(char **filename = &argv[1], **end = &argv[argc];
  filename != end; ++filename) {
    if(**filename == '-') 
      continue;
    FILE *ch = fopen(*filename, "rb");
    if(ch == NULL) {
        printf("%s",argv[0]);
        printf("Творит херню\n");
        continue;
    }
    CatFile(ch,flags,table);
    fclose(ch);
    printf("\n");
    }
}

int main(int argc, char *argv[]) {
 AllFlags flags = ReadFlags(argc,argv);
 const char *table[256];
 CatSetTable(table);
 if(flags.flag_e)
    Flag_E(table);
 if(flags.flag_t)
    Flag_T(table);
 if(flags.flag_v)
    CatSet_V(table);
  Cat(argc,argv,flags,table);
}

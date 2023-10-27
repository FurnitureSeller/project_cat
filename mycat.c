#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cat_functions.h"
#include <errno.h>


AllFlags ReadFlags(int argc,char **argv) { //дожна только сказать какие флаги задействуются
  struct option longopts[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0}
      }; 
  int FlagNow = 0;
  AllFlags SensorFlags = {0, 0, 0, 0, 0, 0};

  while ((FlagNow = getopt_long(argc, argv, "bevEnstT", longopts, NULL)) != -1) {
    switch (FlagNow) {
    case 'b':
    SensorFlags.flag_b = 1;
    break; case 'e':
    SensorFlags.flag_e = 1;
    case 'v':
    SensorFlags.flag_v = 1;
    break; case 'E':
    SensorFlags.flag_e = 1;
    break; case 'n':
    SensorFlags.flag_n = 1;
    break; case 's':
    SensorFlags.flag_s = 1;
    break; case 't':
    SensorFlags.flag_v = 1;
    case 'T':
    SensorFlags.flag_t = 1;
    }
  }
  return SensorFlags;
}

void CatFile(FILE *file, AllFlags flags,const char *table[static 256])
{
  int c = 0;
  int lines = 0;
  int last;
  (void) flags;
  while(fread(&c,1,1,file) > 0) { 
    if(c == '\n') {
      if(flags.flag_s && c == '\n')
      continue;
      if (flags.flag_n) {
        printf("%6i  ",lines++);
      }
      else if(flags.flag_b) {
        if(c != '\n')
          printf("%6i  ",lines++);
      }
    }
  if(c == '0')
    printf("\0");
  printf("%s",table[c]);
  last = c;
  }
}

void Cat(int argc,char *argv[],AllFlags flags,const char *table[static 256]) {
  for(char **filename = &argv[1], **end = &argv[argc];
  filename != end; filename++) {
    FILE *ch = 0;
    if(**filename == '-') {
      continue;
    fopen(*filename, "rb");
    if(ch) {
        fprintf("%s",argv[0]);
        perror(*filename);
        continue;
    }
    CatFile(ch,flags,table);
    fclose(ch);
    }
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>


typedef struct {
    int regex_flag;
    bool invert;
    bool count;
    bool fileMatch;
    bool numberline;
} Flags;

Flags ReadFlags(int argc,char **argv) { 
Flags flags = {0, false, false, false, false};
    int FlagNow = 0;
  while ((FlagNow = getopt(argc, argv, "eivcln")) != -1) {
    switch (FlagNow) {
        break; case 'e':
            flags.regex_flag |= REG_EXTENDET;
        break; case 'i':
            flags.regex_flag |= REG_ICASE;
        break; case 'v':
            flags.invert = true;
        break; case 'c':
            flags.count = true;
        break; case 'l':
            flags.fileMatch = true;
        break; case 'n':
            flags.numberline = true;
    }
  }
  return flags;
}

void Grep(int argc, char **argv, Flags flags);

int main(int argc,char *argv[]) {
    Flags flags = ReadFlags(argc,argv);
}
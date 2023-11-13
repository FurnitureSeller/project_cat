#define _GNU_SOURCE //getline
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int regex_flag;
    bool invert;
    bool count;
    bool filesMatch;
    bool numberLine;
    bool printMatched;
} AllFlags;

AllFlags ReadFlags(int argc,char **argv) {  
  AllFlags flags = {0, false, false, false,false,false};
  int FlagNow = 0;                
  while ((FlagNow = getopt(argc, argv, "eivclno")) != -1) {
    switch (FlagNow) {
    case 'e':
        flags.regex_flag |= REG_EXTENDED;
    break; case 'i':
        flags.regex_flag |= REG_ICASE;
    break; case 'v':
        flags.invert = true;
    break; case 'c':
        flags.count = true;
    break; case 'l':
        flags.filesMatch = true;
    break; case 'n':
        flags.numberLine = true;
    break; case 'o':
        flags.numberLine = true;
    break; 
    }
  }
  return flags;
}
void GrepCount (FILE *file, char const *filename, AllFlags flags,regex_t *preg,int count_f) {
    char *line = 0;
    (void) filename;
    size_t length = 0;
    regmatch_t match;
    int count = 0;
    while(getline(&line,&length,file) > 0) {
        if((!regexec(preg,line,1,&match,0))) {
            count++;
        }
    }
    if(count_f == 1)
        printf("%i\n",count);
    else
        printf("%s:%i\n",filename,count_f);
    free(line);
}
void GrepFile(FILE *file, AllFlags flags,regex_t *preg,char *filename){
    char *line = 0;
    size_t length = 0;
    regmatch_t match;
    int count = 0;
    while(getline(&line,&length,file) > 0) {
        count++;
     if(flags.invert) {
        if(regexec(preg,line,1,&match,0) ) {
            if(flags.printMatched)
            ;
            else { 
                if(flags.numberLine)
                    printf("%s:%i:%s",filename,count,line);
                else
                    printf("%s",line);
            }
        }
     }
     else {
        if(!regexec(preg,line,1,&match,0)) {
            if(flags.printMatched) {
                if(flags.numberLine)
                    printf("%s:%i:%.*s\n",filename,count,match.rm_eo - match.rm_so,line + match.rm_so);
                else
                    printf("%.*s\n",match.rm_eo - match.rm_so,line + match.rm_so);
                char *remaining = line + match.rm_eo;
                while(!regexec(preg,remaining,1,&match,0)) {
                    if(flags.numberLine)
                         printf("%s:%i:%.*s\n",filename,count,match.rm_eo - match.rm_so,line + match.rm_so);
                    else
                        printf("%.*s\n",match.rm_eo - match.rm_so,line + match.rm_so);
                    remaining = line + match.rm_eo;
                }
            }
            else {
                if(flags.numberLine)
                    printf("%s:%i:%s",filename,count,line);
                else
                  printf("%s",line);
            }
        }
     } 
    }
    free(line);
}

void Grep(int argc,char *argv[],AllFlags flags) {
char **pattern = &argv[1];  /*pattern: The text pattern you want to search for.*/
char **end = &argv[argc];   // наш паттерн не сразу патерн 
int count = 0;
regex_t preg_storage;
regex_t *preg = &preg_storage;  //символическая хрень???????
for(;pattern != end && pattern[0][0] =='-';pattern++) //скип флагов
;
if(pattern == end) {            // нету шаблона не предоставлен
    fprintf(stderr,"no pattern\n");
    exit(1);
}                                              /*preg испол для хран скомп регул выраж*/
if(regcomp(preg,*pattern,flags.regex_flag)){         /*pattern указатель на что ввели  */
    fprintf(stderr, "faild to cimpile regex\n"); /* flag это опции*/
    exit(1);
}

for(char **filename = pattern + 1;  // шагает к файлам
  filename != end; ++filename) {
    if(**filename == '-') 
      continue;
    count++;
    if (count >= 2)
      break;
  }
  
for(char **filename = pattern + 1;  // шагает по файлам
  filename != end; filename++) {
    if(**filename == '-') 
      continue;
    FILE *ch = fopen(*filename, "rb");
    if(ch == NULL) {
        printf("%s",*filename);
        printf("Творит дич\n");
        continue;
    }
    if(flags.count) {
        GrepCount(ch,*filename,flags,preg,count);
    }
    else
        GrepFile(ch,flags,preg,*filename);
    fclose(ch);
    }
}

int main(int argc, char *argv[]) {
  AllFlags flags = ReadFlags(argc,argv);
  Grep(argc,argv,flags);
}

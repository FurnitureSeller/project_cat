#define _GNU_SOURCE //getline
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


typedef struct {
    char *pattern;
    size_t size;
    int regex_flag; //e i
    bool invert;
    bool count;
    bool filesMatch;
    bool numberLine;
    bool printMatched;
} AllFlags;


char *string_append_expr(char *string, size_t *size, char const *expr,size_t size_expr) {
    string = realloc(string,*size + size_expr + 7);
    string[*size] = '\\';
    string[*size + 1] = '|';
    string[*size + 2] = '\\';
    string[*size + 3] = '(';
    memcpy(string + *size + 4, expr, size_expr);
    *size += size_expr + 4;
    string[*size] = '\\';
    string[*size + 1] = ')';
    string[*size + 2] = '\0';
    *size += 2;
    return string;
}

AllFlags ReadFlags(int argc,char **argv) {  
  AllFlags flags = {NULL, 0, 0, false, false, false,false,false};
  char FlagNow = getopt_long(argc,argv,"e:ivclno",0,0);
  flags.pattern = malloc(2);
  flags.pattern[0] = '\\';
  flags.pattern[1] = '\0';
  size_t pattern_size = 0;              
  while ((FlagNow = getopt_long(argc,argv,"e:ivclno",0,0)) != -1) {
    printf("optarg = %s\n",optarg);   /*efwefewfgergergrggergergergergergergergreg*/
    switch (FlagNow) {
    case 'e':
        flags.pattern = string_append_expr(flags.pattern,&pattern_size,optarg,strlen(optarg));
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
        flags.printMatched = true;
    }
  }
  if(pattern_size) {
    flags.size = pattern_size;
  }
  return flags;
}
void GrepCount (FILE *file, char const *filename, AllFlags flags,regex_t *preg,int count_file) {
    char *line = 0;
    (void) flags;
    size_t length = 0;
    regmatch_t match;
    int count = 0;
    while(getline(&line,&length,file) > 0) {
        if((!regexec(preg,line,1,&match,0))) {
            count++;
        }
    }
    if(count_file == 1)
        printf("%i\n",count);
    else
        printf("%s:%i\n",filename,count);
    free(line);
}
void GrepFile(FILE *file, AllFlags flags,regex_t *preg,char *filename){
    char *line = 0;
    (void) flags;
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
                         printf("%s:%i:%.*s\n",filename,count,match.rm_eo - match.rm_so,remaining + match.rm_so);
                    else
                        printf("%.*s\n",match.rm_eo - match.rm_so,remaining + match.rm_so);
                    remaining = remaining + match.rm_eo;
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
    char **end = &argv[argc];
    regex_t preg_storage;
    regex_t *preg = &preg_storage;
    if(flags.size == 0) {
        if(regcomp(preg,argv[0],flags.regex_flag)) {
            fprintf(stderr,"faild to cimpile regex\n");
            exit(1);
        }
    }
    else {                                  
        if(regcomp(preg, flags.pattern + 2, flags.regex_flag)){        
          fprintf(stderr, "faild to cimpile regex\n"); 
          exit(1);
    }
}
free(flags.pattern);
if(argc == (flags.size ? 2 : 1)) {
    if(flags.count) {
        GrepCount(stdin,"",flags,preg,1);
    }
    else
        GrepFile(stdin,flags,preg,"");
}
for(char **filename = argv + (flags.size ? 0 : 1);filename != end; ++filename) {
    FILE *ch = fopen(*filename, "rb");
    if(ch == NULL) {
        printf("%s",*filename);
        printf("Творит дич\n");
        continue;
    }
    if(flags.count) {
        GrepCount(ch,*filename,flags,preg,argc);
    }
    else
        GrepFile(ch,flags,preg,*filename);
    fclose(ch);
    }
  regfree(preg);
}

int main(int argc, char *argv[]) {
  AllFlags flags = ReadFlags(argc,argv);
  printf("%d",optind);
  argv += optind;     /*изначально равен 1*/
  argc -= optind;       /*содержит указатель на текущее значение, связанное с опцией, требующей аргумент*/
  if(argc == 0) {       /*В этом примере, после обработки всех опций */
    fprintf(stderr,"no pattern\n"); /*optind будет указывать на первый аргумент, который не является опцией.*/
    exit(1);
  }
  Grep(argc,argv,flags);
}

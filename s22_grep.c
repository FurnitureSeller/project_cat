#define _GNU_SOURCE //getline
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "s22_grep.h"


typedef struct {
    char *Mpattern;  //подобие динамической строки
    size_t Msize;
    int regex_flag; 
    bool invert;
    bool count;
    bool filesMatch;
    bool numberLine;
    bool printMatched;
} AllFlags;

AllFlags ReadFlags(int argc,char **argv) {  
  AllFlags flags = {NULL, 0, 0, false, false, false,false,false};
  char FlagNow = 0; //getopt_long(argc,argv,"e:ivclno",0,0);
  flags.Mpattern = malloc(2);
  flags.Mpattern[0] = '\\';
  flags.Mpattern[1] = '\0';
  size_t pattern_size = 0;              
  while ((FlagNow = getopt_long(argc,argv,"e:ivclno",0,0)) != -1) {
    switch (FlagNow) {
    case 'e':
        flags.Mpattern = Optargunion(flags.Mpattern,&pattern_size,optarg,strlen(optarg)); 
    break; case 'i':            /*вернула динам стрк в структуру и ее размер(косвенно,ниже точн)*/
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
    flags.Msize = pattern_size;  //размер дин стрки вернули в структуру
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
void GrepFile(FILE *file, AllFlags flags,regex_t *preg,char *filename,int count_files) {
    char *line = NULL; /* Null l added*/
    (void) flags;
    size_t length = 0; //сама ему поддаст
    regmatch_t match;
    int strokaCounter = 0;
    while(getline(&line,&length,file) > 0) { //три аргумента: указатель на строку, в которую будет записываться ввод, указатель на переменную, которая хранит размер буфера строки, и файловый дескриптор ввода
        strokaCounter++;
     if(flags.invert) {  /* ЕСЛИ ФЛАГ V*/
        if(regexec(preg,line,1,&match,0) ) { //что за match!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(flags.printMatched)
            ;
            else { 
                if(flags.numberLine) //ЕСЛИ N
                    printf("%s:%i:%s",filename,strokaCounter,line);
                if(count_files >= 2) {                                         //СОМНЕВАЮСЬ
                    printf("%s:%i:%s",filename,strokaCounter,line);
                }
                else
                    printf("%s",line);
            }
        }
     } // 5 арг-указ на ском рег выраж, строку, котор проверить, кол элем в массиве, указывающем на подвыражения, массив, указывающий на подвыражения, и флаги, которые могут изменять поведение сопоставления */

     else { /* Если флаг не V*/
        if(!regexec(preg,line,1,&match,0)) {   
            if(flags.printMatched) {  // Флаг -О
                    if(flags.numberLine)  // если ФЛАГ -N
                        printf("%s:%i:%.*s\n",filename,strokaCounter,match.rm_eo - match.rm_so,line + match.rm_so);
                    else   // КАКОЙ ЕЩЕ ДРУГОЙ СЛУЧАЙ?
                        printf("%.*s\n",match.rm_eo - match.rm_so,line + match.rm_so);
                        /*ЧТО ЭТООООООО*/
                    char *remaining = line + match.rm_eo;
                    while(!regexec(preg,remaining,1,&match,0)) {
                        if(flags.numberLine)
                            printf("%s:%i:%.*s\n",filename,strokaCounter,match.rm_eo - match.rm_so,remaining + match.rm_so);
                        else
                            printf("%.*s\n",match.rm_eo - match.rm_so,remaining + match.rm_so);
                        remaining = remaining + match.rm_eo;
                    }
                }
            else {
                    if(flags.numberLine) // если ФЛАГ -N
                        printf("%s:%i:%s",filename,strokaCounter,line);
                    else
                    printf("%s:%s",filename,line); //печатает ответ без флагов!
                }
        }
     } 
    }
    free(line);
}

void Grep(int argc,char *argv[],AllFlags flags,int count_files) {
    char **end = &argv[argc];
    regex_t preg_storage;
    regex_t *preg = &preg_storage;
    if(flags.Msize == 0) {    //В МЕЙН интересно что будет без паттерна // 
        if(regcomp(preg,argv[0],flags.regex_flag)) { //а точно ли строка это argv[0]
            fprintf(stderr,"faild to cimpile regex\n");
            exit(1);
        }
    }
    else {                  /*+2 тип строка \0???*/                
        if(regcomp(preg, flags.Mpattern + 2, flags.regex_flag)){    //зачем ей флаг?    
          fprintf(stderr, "faild to cimpile regex\n"); 
          exit(1);
    }
}
free(flags.Mpattern);

for(char **filename = argv + (flags.Msize ? 0 : 1);filename != end; ++filename) {
    FILE *ch = fopen(*filename, "rb");
    if(ch == NULL) {
        printf("%s",*filename);
        printf("Творит дич\n");
        continue;
      }
    if(flags.count) {
        GrepCount(ch,*filename,flags,preg,argc); //ФЛАГ -C ?????
      }
    else
        GrepFile(ch,flags,preg,*filename,count_files);
        fclose(ch);
  }
    regfree(preg);
}

int main(int argc, char *argv[]) {
  int count_files = 0;
  AllFlags flags = ReadFlags(argc,argv);
  count_files = argc - optind - 1;
  printf("optind = %d\n",optind);
  printf("argc = %d\n",argc);
  printf("N files %d\n",count_files);
  checkPattern(&argc,&argv,optind);
  Grep(argc,argv,flags,count_files);
  return 0;
}


// if(argc == (flags.Msize ? 2 : 1)) {   // флаг -с
//     if(flags.count) {
//         GrepCount(stdin,"",flags,preg,1); //выводит только кол совпад строк
//     }
//     else
//         GrepFile(stdin,flags,preg,""); //(FILE *file, AllFlags flags,regex_t *preg,char *filename)
// }

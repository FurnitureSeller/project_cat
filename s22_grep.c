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
    bool onlyline;
    bool nonstop;
    char *patternfile;
    size_t Fsize;
} AllFlags;

int checkPattern(int *argc, char **argv[],int optind,AllFlags flags) {
  int count_files = 0;
  *argv += optind;    //остается количество файлов
  *argc -= optind;      
  if(*argc == 0) {      
    fprintf(stderr,"no pattern\n"); 
    exit(1);
  }
  if(flags.Msize || flags.Fsize) 
    count_files = *argc;
  else
    count_files = *argc - 1;

  return count_files;
}

AllFlags ReadFlags(int argc,char **argv) {  
  AllFlags flags = {NULL, 0, 0, false, false, false,false,false,false,false,NULL,0};
  char FlagNow = 0; 
  flags.Mpattern = malloc(2);
  flags.Mpattern[0] = '\\';
  flags.Mpattern[1] = '\0';
  size_t pattern_size = 0; 

  flags.patternfile = malloc(2);
  flags.patternfile[0] = '\\';
  flags.patternfile[1] = '\0';
  size_t fpattern_size = 0; 

  while ((FlagNow = getopt_long(argc,argv,"e:ivclnohsf:",0,0)) != -1) {
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
    break; case 'h':
        flags.onlyline = true;
    break;case 's':
        flags.nonstop = true;
    break;case 'f':
        flags.patternfile = filePattern(flags.patternfile, &fpattern_size, optarg);
    break;
    }
  }
  if(pattern_size) {
    flags.Msize = pattern_size;  
  }
  if(fpattern_size) {
    flags.Fsize = fpattern_size;  
  }
  return flags;  
}

/*нужен для флага C*/
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
    char *line = NULL; 
    (void) flags;
    size_t length = 0; 
    regmatch_t match;
    int strokaCounter = 0;
    int Match_helper = 1;
    while(getline(&line,&length,file) > 0) { //три аргумента: указатель на строку, в которую будет записываться ввод, указатель на переменную, которая хранит размер буфера строки, и файловый дескриптор ввода
        strokaCounter++;
        line[strcspn(line, "\n")] = '\0';
     if(flags.invert) {  /* ЕСЛИ ФЛАГ V*/
        if(regexec(preg,line,1,&match,0) ) { 
          if(count_files >= 2)                                         
            printf("%s:%s\n",filename,line);
          else
            printf("%s\n",line);
        }
     } // 5 арг-указ на ском рег выраж, строку, котор проверить, кол элем в массиве, указывающем на подвыражения, массив, указывающий на подвыражения, и флаги, которые могут изменять поведение сопоставления */

     else { /* Если флаг не V*/
        if(!regexec(preg,line,1,&match,0)) {   
        if(flags.printMatched) {  // Флаг -О     
          if(count_files >= 2)  
            printf("%s:%.*s\n",filename,match.rm_eo - match.rm_so,line + match.rm_so);
           else
            printf("%.*s\n",match.rm_eo - match.rm_so,line + match.rm_so);
            char *remaining = line + match.rm_eo;
            while(!regexec(preg,remaining,1,&match,0)) {
              if(count_files >= 2)  
                printf("%s:%.*s\n",filename,match.rm_eo - match.rm_so,remaining + match.rm_so);
              else
                printf("%.*s\n",match.rm_eo - match.rm_so,remaining + match.rm_so);
              remaining = remaining + match.rm_eo;
            }
        }
            
            else {
                    if(flags.numberLine) // если ФЛАГ -N
                        printf("%s:%i:%s\n",filename,strokaCounter,line);
                    else if(flags.filesMatch && Match_helper == 1) {
                        printf("%s\n",filename); 
                        Match_helper = 0; 
                        }
                    else if(flags.onlyline) {
                        printf("%s\n",line);
                    }
                    else {
                        if(!flags.filesMatch)
                            printf("%s:%s\n",filename,line); //печатает ответ без флагов!
                    }
                }
        }
     } 
    }
    free(line);
}

void Patterfinder(FILE *file, AllFlags flags,regex_t *preg,char *filename,int count_files) { 
    char *line = NULL;
    size_t length = 0;
    regmatch_t match;
    int strokaCounter = 0;
    int coincid = 0;

  while((getline(&line,&length,file) > 0) && coincid == 0) { //три аргумента: указатель на строку, в которую будет записываться ввод, указатель на переменную, которая хранит размер буфера строки, и файловый дескриптор ввода
    line[strcspn(line, "\n")] = '\0';
      if(!regexec(preg,line,1,&match,0)) 
        coincid = 1;  
      }
  if (coincid == 1) {
    rewind(file);
    while(getline(&line,&length,file) > 0) { //три аргумента: указатель на строку, в которую будет записываться ввод, указатель на переменную, которая хранит размер буфера строки, и файловый дескриптор ввода
      strokaCounter++;
      line[strcspn(line, "\n")] = '\0';
        if(count_files >= 2)                                         //СОМНЕВАЮСЬ
          printf("%s:%s\n",filename,line);
        else
              printf("%s\n",line);
    }
  } 
  free(line);
}

void Grep(int argc,char *argv[],AllFlags flags,int count_files) {
    char **end = &argv[argc];
    regex_t preg_storage;
    regex_t *preg = &preg_storage;
    if(flags.Msize == 0 && flags.Fsize == 0) {    //В МЕЙН интересно что будет без паттерна // 
        if(regcomp(preg,argv[0],flags.regex_flag)) { //а точно ли строка это argv[0]
            fprintf(stderr,"faild to cimpile regex\n");
            exit(1);
        }
    }
    else if(flags.Msize) {                  /*+2 тип строка \0???*/                
        if(regcomp(preg, flags.Mpattern + 2, flags.regex_flag)){    //зачем ей флаг?    
          fprintf(stderr, "faild to cimpile regex\n"); 
          exit(1);
        }
    }
    else if(flags.patternfile){
        if(regcomp(preg, flags.patternfile + 2, flags.regex_flag)){    //зачем ей флаг?    
          fprintf(stderr, "faild to cimpile regex\n"); 
          exit(1);
        }
    }       /*ДЕФОЛТНЫЙ СЛУЧАЙ*/
   free(flags.Mpattern);
   free(flags.patternfile);
    int sdvig = 1;
    if(flags.Msize || flags.Fsize)
      sdvig = 0;
   
    for(char **filename = argv + sdvig;filename != end; ++filename) {
        FILE *ch = fopen(*filename, "rb");
        if(ch == NULL) {
            if(flags.nonstop)
                continue;
            else {
            printf("%s",*filename);
            printf("Творит дич\n");
            continue;
            }
        }
        if(flags.count) {
            GrepCount(ch,*filename,flags,preg,count_files); //ФЛАГ -C ?????
        }
        else if (flags.Fsize) {
            Patterfinder(ch,flags,preg,*filename,count_files);
            fclose(ch);
        }
        
        else {
            GrepFile(ch,flags,preg,*filename,count_files);
            fclose(ch);
        }
    }
        regfree(preg);
}

int main(int argc, char *argv[]) {
  int count_files = 0;
  AllFlags flags = ReadFlags(argc,argv);
  count_files = checkPattern(&argc,&argv,optind,flags);
  Grep(argc,argv,flags,count_files);
  return 0;
}

#define _GNU_SOURCE //getline
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


typedef struct {
    char *Mpattern;  //подобие динамической строки
    size_t Msize;
    int regex_flag; //e i
    bool invert;
    bool count;
    bool filesMatch;
    bool numberLine;
    bool printMatched;
} AllFlags;


char *Optargunion(char *OneDinstring, size_t *Msize, char const *target,size_t size_target) {
    OneDinstring = realloc(OneDinstring,*Msize + size_target + 7);
    OneDinstring[*Msize] = '\\';
    OneDinstring[*Msize + 1] = '|';
    OneDinstring[*Msize + 2] = '\\';
    OneDinstring[*Msize + 3] = '('; /*расширяем мегагурендан*/
    memcpy(OneDinstring + *Msize + 4, target, size_target); //три аргумента: указатель на целевую область памяти, указатель на исходную область памяти и количество байтов для копирования
    *Msize += size_target + 4;
    OneDinstring[*Msize] = '\\';
    OneDinstring[*Msize + 1] = ')';
    OneDinstring[*Msize + 2] = '\0';
    *Msize += 2;
    return OneDinstring;
}

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
void GrepFile(FILE *file, AllFlags flags,regex_t *preg,char *filename){
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

void Grep(int argc,char *argv[],AllFlags flags) {
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

// if(argc == (flags.Msize ? 2 : 1)) {   // флаг -с
//     if(flags.count) {
//         GrepCount(stdin,"",flags,preg,1); //выводит только кол совпад строк
//     }
//     else
//         GrepFile(stdin,flags,preg,""); //(FILE *file, AllFlags flags,regex_t *preg,char *filename)
// }


for(char **filename = argv + (flags.Msize ? 0 : 1);filename != end; ++filename) {
    FILE *ch = fopen(*filename, "rb");
    if(ch == NULL) {
        printf("%s",*filename);
        printf("Творит дич\n");
        continue;
      }
    if(flags.count) {
        GrepCount(ch,*filename,flags,preg,argc); //ФЛАГ -l ?????
      }
    else
        GrepFile(ch,flags,preg,*filename);
        fclose(ch);
  }
    regfree(preg);
}

int main(int argc, char *argv[]) {
  AllFlags flags = ReadFlags(argc,argv);
  printf("optind = %d\n",optind);

  printf("%s\n",argv[optind+1]);
  argv += optind;    //остается количество файлов
  printf("%s\n",argv[1]);
  printf("%s\n",argv[0]);

  argc -= optind;      
  if(argc == 0) {      
    fprintf(stderr,"no pattern\n"); 
    exit(1);
  }
  Grep(argc,argv,flags);
  return 0;
}

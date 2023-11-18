#include "s22_grep.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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


char *filePattern(char *lineofPatterns, size_t *sizeLp,char const *target) {
  FILE *ch = fopen(target, "rb");
    if(ch == NULL) {
        printf("%s",target);
        printf("Его НЕТ НИГДЕ!!\n");
      }
  char *line = NULL; 
  size_t length = 0;
  int many = 0;

  while((many = getline(&line,&length,ch)) > 0) {
    if(*line != '\n') {
    line[strcspn(line, "\n")] = '\0';
    lineofPatterns = realloc(lineofPatterns,*sizeLp + many + 7);
    lineofPatterns[*sizeLp] = '\\';
    lineofPatterns[*sizeLp + 1] = '|';
    lineofPatterns[*sizeLp + 2] = '\\';
    lineofPatterns[*sizeLp + 3] = '('; /*расширяем мегагурендан*/
    memcpy(lineofPatterns + *sizeLp + 4, line, many); //три аргумента: указатель на целевую область памяти, указатель на исходную область памяти и количество байтов для копирования
    *sizeLp += (many + 3);
    lineofPatterns[*sizeLp] = '\\';
    lineofPatterns[*sizeLp + 1] = ')';
    lineofPatterns[*sizeLp + 2] = '\0';
    *sizeLp += 2;
     }
  }
  free(line);
  return lineofPatterns;
}


// void Patterfinder(FILE *file, AllFlags flags,regex_t *preg,char *filename,int count_files) { 
//   char *line = NULL; 
//     size_t length = 0;
//     regmatch_t match;
//     int strokaCounter = 0;
//     int Match_helper = 1;
//     int coincid = 0;

//   while(getline(&line,&length,file) > 0) { //три аргумента: указатель на строку, в которую будет записываться ввод, указатель на переменную, которая хранит размер буфера строки, и файловый дескриптор ввода
//     line[strcspn(line, "\n")] = '\0';
//       if(!regexec(preg,line,1,&match,0)) 
//         coincid = 1;  
//       }

//   if (coincid == 1) {
//     while(getline(&line,&length,file) > 0) { //три аргумента: указатель на строку, в которую будет записываться ввод, указатель на переменную, которая хранит размер буфера строки, и файловый дескриптор ввода
//       strokaCounter++;
//       line[strcspn(line, "\n")] = '\0';
//         if(!regexec(preg,line,1,&match,0)) {
//            if(count_files >= 2) {                                         //СОМНЕВАЮСЬ
//             printf("%s:%i:%s",filename,strokaCounter,line);
//                 }
//             else
//               printf("%s",line);
//         }  
//     }
//   } 
// }
#include "s22_grep.h"

void checkPattern(int *argc, char **argv[],int optind) {
  *argv += optind;    //остается количество файлов
  *argc -= optind;      
  if(*argc == 0) {      
    fprintf(stderr,"no pattern\n"); 
    exit(1);
  }
}

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

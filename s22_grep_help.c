#include "s21_grep.h"

void check_pattern(int argc,char **argv,int optind) {
  argv += optind;     /*изначально равен 1*/
  argc -= optind;       /*содержит указатель на текущее значение, связанное с опцией, требующей аргумент*/
  if(argc == 0) {       /*В этом примере, после обработки всех опций */
    fprintf(stderr,"no pattern\n"); /*optind будет указывать на первый аргумент, который не является опцией.*/
    exit(1);
  }
}

char *string_append_expr(char *string, size_t *size, char const *expr,size_t size_expr) {
    string = realloc(string,*size + size_expr + 7);
    string[*size] = '\\';
    string[*size+1] = '|';
    string[*size+2] = '\\';
    string[*size + 3] = '(';
    memcpy(string + *size + 4,expr,size_expr);
    *size += size_expr + 4;
    string[*size] = '\\';
    string[*size + 1] = ')';
    string[*size + 2] = '\0';
    *size += 2;
    return string;
}
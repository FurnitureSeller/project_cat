#include "s22_grep.h"

void FileDistrib() {
  if(argc == (flags.Msize ? 2 : 1)) {   // флаг -с
    if(flags.count) {
        GrepCount(stdin,"",flags,preg,1); //выводит только кол совпад строк
    }
    else
        GrepFile(stdin,flags,preg,"");
}
}
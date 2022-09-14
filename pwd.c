#include "headers.h"

extern int maxPathSize;

void pwd()
{
    char buf[maxPathSize];
    getcwd(buf, maxPathSize);
    printf("%s\n",buf);
}
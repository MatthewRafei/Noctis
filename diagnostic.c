#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "diagnostic.h"

void report_error(const char *file, const int line, const int col, const enum ErrorLevel level, const char *fmt)
{
    (void)file;
    (void)line;
    (void)col;
    (void)level;
    (void)fmt;

    switch (level)
    {
    case INFO:
        fprintf(stderr, fmt, file,line,col,level);
        break;
    
    default:
        break;
    }

    

}
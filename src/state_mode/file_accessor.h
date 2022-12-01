#ifndef __FILE_ACCESSOR_H__
#define __FILE_ACCESSOR_H__
#include <stdio.h>

typedef struct FileAccessorContext {
    const char * const pFname;
    const char * const pMode;
    void (* const processor) (struct FileAccessorContext *pThis, FILE *fp);
} FileAccessorContext;

bool access_file(FileAccessorContext *pCtx);
#endif
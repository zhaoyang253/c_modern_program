#ifndef __FILE_READER_H__
#define __FILE_READER_H__
#include <stdio.h>

typedef struct FileReaderContext {
    const char * const pFname;
    void (* const processor)(struct FileReaderContext *pThis, FILE *fp);
} FileReaderContext;

int read_file(FileReaderContext pCtx);

#endif
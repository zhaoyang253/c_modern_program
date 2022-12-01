#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <stdio.h>

typedef struct BufferContext {
    void *pBuf;
    long size;
    void (* processor)(struct BufferContext *pThis);
} BufferContext;

bool buffer(BufferContext *pThis);

#endif

#include "file_accessor.h"

bool access_file(FileAccessorContext *pCtx)
{
    FILE *fp = fopen(pCtx->pFname, pCtx->pMode);

    if (fp == NULL) return false;

    pCtx->processor(pCtx, fp);

    close(fp);

    return true;
}

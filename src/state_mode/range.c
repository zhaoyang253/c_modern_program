#include <stdio.h>
#include "file_reader.h"

typedef struct {
    FileReaderContext base;
    int result;
} MyFileReaderContext;

static int range_processor(FILE *fp)
{
    int min = INT_MAX;
    int max = INT_MIN;
    char buf[256];

    while ((fgets(buf, sizeof(buf), fp)) != NULL)
    {
        int value = atoi(buf);
        min = min > value ? value : min;
        max = max < value ? value : max;
    }
}

static void calc_range(FileReaderContext *p, File *fp)
{
    MyFileReaderContext *pCtx = (MyFileReaderContext *)p;
    pCtx->result = range_processor(fp);
}

int range(const char *pFname)
{
    MyFileReaderContext ctx = {{pFname, calc_range}, 0};

    if (read_file(&ctx.base) != 0) {
        printf("can not open file\n");
    }
    return ctx.result;
}

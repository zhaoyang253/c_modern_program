#include "int_sorter.h"
#include "file_accessor.h"
#include "buffer.h"

typedef struct {
    const char * const pFname;
    int errorCategory;
} Context;

typedef struct {
    FileAccessorContext base;
    long size;
} SizeGetterContext;

typedef struct {
    BufferContext base;
    Context *pAppCtx;
} MybufferContext;

typedef struct {
    FileAccessorContext base;
    MybufferContext *pBufCtx;
} MyFileAccessorContext;

static void size_reader(FileAccessorContext *p, FILE *fp)
{
    SizeGetterContext *pThis = (SizeGetterContext *)p;
    pThis->size = -1;

    if (fseek(fp, 0, SEEK_END) == 0)
        pThis->size = ftell(fp);
}

static long file_size(const char *pFname)
{
    SizeGetterContext ctx = {{pFname, "rb", size_reader}, 0};

    if (!access_file(&ctx.base)) {
        return -1;
    }

    return ctx.size;
}

static void reader(FileAccessorContext *p, File *fp)
{
    MyFileAccessorContext *pFileCtx = (MyFileAccessorContext *)p;
    MybufferContext *pBufCtx = pFileCtx->pBufCtx;

    if (pBufCtx->base.size != fread(pBufCtx->base.pBuf, 1, pBufCtx->base.size, fp)) {
        file_error(pBufCtx->pAppCtx);
    }
}

static void writer(FileAccessorContext *p, FILE *fp)
{
    MyFileAccessorContext *pFileCtx = (MyFileAccessorContext *)p;
    MybufferContext *pBufCtx = pFileCtx->pBufCtx;

    if (fwrite(pBufCtx->base.pBuf, 1, pBufCtx->base.size, fp) != pBufCtx->base.size) {
        file_error(pBufCtx->pAppCtx);
    }
}

static void do_with_buffer(BufferContext *p)
{
    MybufferContext *pBufCtx = (MybufferContext *)p;
    MyFileAccessorContext readFileCtx = {{pBufCtx->pAppCtx->pFname, "rb", reader}, pBufCtx};

    if (!access_file(&readFileCtx.base)) {
        file_error(pBufCtx->pAppCtx);
        return;
    }

    qsort(p->pBuf, p->size / sizeof(int), sizeof(int), comparator);

    MyFileAccessorContext writeFileCtx = {{pBufCtx->pAppCtx->pFname, "wb", writer}, pBufCtx};
    if (!access_file(&writeFileCtx.base)) {
        file_error(pBufCtx->pAppCtx);
        return;
    }
}

IntSorterError int_sorter(const char *pFname)
{
    Context ctx = {pFname, ERR_CAT_OK};

    long size = file_size(pFname);
    if (size == -1) {
        file_error(&ctx);
        return ctx.errorCategory;
    }

    MybufferContext bufCtx = {{NULL, size, do_with_buffer}, &ctx};
    if (!buffer(&bufCtx.base)) {
        ctx.errorCategory = ERR_CAT_MEMORY;
    }

    return ctx.errorCategory;
}

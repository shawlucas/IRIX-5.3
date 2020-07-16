/*
 *  UNIX shell
 */

#include <memory.h>
#include "defs.h"

/*
 * storage allocator
 * (circular first fit strategy)
 */

#define BUSY 01
#define busy(x) (Rcheat((x)->ag.word) & BUSY

unsigned int brkincr = BRKINCR;
static struct blk* blokp; /* current search pointer */
static struct blk* bloktop; /* top of arena (last blok) */

static unsigned char *brkbegin, *aligned_brkbegin;
unsigned char* setbrk();
void addblok();

#ifdef __STDC__
void *
#else
char *
#endif
alloc(size_t nbytes)
{
    for (;;)
    {
        int c = 0;
        register struct blk* p = blokp;
        register struct blk* q;

        do
        {
            if (!busy(p))
            {
                while (!busy(q = p->ag.word))
                    p->ag.word = q->ag.word;
            }
        }
    }   
}
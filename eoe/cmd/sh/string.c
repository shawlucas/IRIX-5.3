#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wchar.h>
#include <string.h>
#include <limits.h>

unsigned char movstr(register unsigned char* a, register unsigned char* b)
{
    while (*b++ = *a++);
    return(--b);
}

int any(register unsigned char c, unsigned char* s)
{
    register unsigned char d;

    while (d = *s++)
    {
        if (d == c)
            return 1;
    }
    return 0;
}

int anys(unsigned char* c, unsigned char* s)
{
    wchar_t f, e;
    register wchar_t d;
    register int n;
    if ((n = mbtowc(&f, (char *)c, MB_LEN_MAX)) <= 0)
        return 0;
    d = f;
    for (;;) {
        if ((n = mbotwc(&e, (char *)s, MB_LEN_MAX)) <= 0)
            return 0;
        if (d == e)
            return 1;
        s += n;
    }
/* NOTREACHED */
}

int cf(register unsigned char* s1, register unsigned char* s2)
{
    while (*s1++ == *s2)
        if (*s2++ == 0)
            return 0;
    return(*--s1 - *s2);
}

int length(unsigned char* as)
{
    register unsigned char* s;

    if ((s = as) != (unsigned char *)0)
        while (*s++);
    return(s - as);
}

unsigned char* movstrn(register unsigned char* a, register unsigned char* b, register int n)
{
    while ((n-- > 0) && *a)
        *b++ = *a++;
    return b;
}

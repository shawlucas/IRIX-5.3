#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#ifdef __sgi__
    #include <deflt.h>
#endif
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <fmtmsg.h>
#include <locale.h>

#define USER    0700   /* user's bits */
#define GROUP   0070   /* group's bits */
#define OTHER   0007   /* other's bits */
#define ALL     0777   /* all */

#define READ    0444   /* read permit */
#define WRITE   0222   /* write permit */
#define EXEC    0111   /* exec permit */

#define ERR_PERMS  9999

#define MKDIR_LABEL "UX:mkdir"

static mode_t parse_mode(), abs_val(), who(), perms();
static int what();
extern int mkdirp(const char* d, mode_t mode);

char *ms;
long classification = MM_PRINT | MM_SOFT | MM_APPL | MM_RECOVER;

extern int opterr, optind;
extern char *optarg;

int main(int argc, char* argv[])
{
    int pflag, mflag, errflg;
    int c, local_errno, saverr = 0;
    mode_t mode;
    int um_orig;
    char *p, *m;
    char *endptr;
    char *d; /* the (path)name of the directory to be created */
    int err;
    void cleandir();

    (void)setlocale(LC_ALL, "");
    errno = 0; /* temporary soln., reset errno in case setlocale fails*/

    mode = S_IRWXU | S_IRWXG | S_IRWXO;
    pflag = mflag = errflg = 0;
    local_errno = 0;

    um_orig = umask(0); /* save original umask and restore later */
    umask(um_orig);

    while ((c=getopt(argc, argv, "m:p")) != EOF) {
        switch (c) {
            case 'm':
                mflag++;
                ms = optarg;
                mode = parse_mode();
                break;
            case 'p':
                pflag++;
                break;
            case '?':
                errflg++;
                break;
        }
    }

    argc -= optind;
    if (argc < 1 || errflg) {
        if (!errflg) {
            fmtmsg(classification, MKDIR_LABEL, MM_ERROR, "Incorrect usage", "Usage: mkdir [-m mode] [-p] dirname ...", MM_NULLTAG);
        }
        exit(2);
    }

    argv = &argv[optind];

        while(argc--) {
            d = *argv++;
            /* Skip extra slahes at the end of path */
            while ((endptr=strrchr(d, '/')) != NULL) {
                p = endptr;
                p++;
                if (*p == '0')
                    *endptr = '\0';
                else
                    break;
            }

        /* When -p is set, invokes mkdirp library routine.
		 * Although successfully invoked, mkdirp sets errno to ENOENT 
		 * if one of the directory in the pathname does not exist,
		 * thus creates a confusion on success/failure status 
		 * possibly checked by the calling routine or shell. 
		 * Therefore, errno is reset only when
		 * mkdirp has executed successfully, otherwise save
		 * in local_errno.
		 */ 

        if (pflag) {
            mode_t um_temp, fin_mode;
            struct stat stb;

            um_orig = umask(0);
            um_temp = um_orig & 0x477; /* u+wx */
            fin_mode = ALL & ~um_orig;
            umask(um_temp);
            err = stat(d, &stb);
            if (err == 0 && S_ISDIR(stb.st_mode))
                ;
            else if (err == 0) {
                local_errno = ENOTDIR;   
                fprintf(stderr, "UX:mkdir: ERROR: Cannot create directory \"%s\": %s\n", d, strerror(ENOTDIR));
            } else {
                err = mkdirp(d, mode);
                if (err < 0) {
                    fprintf(stderr, "UX:mkdir: ERROR: Cannot create directory \"%s\": %s\n", d, strerror(errno));
                    local_errno = errno;
                }
                if (mflag)
                    err = chmod(d, mode);
                else
                    err = chmod(d, fin_mode);
                if (err < 0) {
                    fprintf(stderr, "UX:mkdir: ERROR: Cannot create directory \"%s\": %s\n", d, strerror(errno));
                    local_errno = errno;
                }
            }
            errno = 0;
            /* No -p. Make only one directory */
        } else {
            err = mkdir(d, mode);
            if (err)
                fprintf(stderr, "UX:mkdir: ERROR: Cannot create directory \"%s\": %s\n", d, strerror(errno));
        }

        if (err) {
            saverr = 2;
            errno = 0;
        }
    } /* end while */

    if (saverr)
        errno = saverr;
    
    /* When pflag is set, the errno is saved in local_errno */
    if (errno == 0 && local_errno)
        errno = local_errno;
    
    umask(um_orig); /* restore original umask value */

    exit(errno ? 2: 0);
    /* NOTREACHED */
} /* main() */

static mode_t parse_mode() {
    mode_t m, b, mask, mode_r = 0777;
    register int o;
    int um;

    um = umask(0);
    if (*ms >= '0' && *ms <= '7')
        return abs_val(ms);
    
    mode_r &= ~um;

    do {
        if (! (m = who())) {
            fmtmsg(classification, MKDIR_LABEL, MM_ERROR, ":14:Invalid mode\n", MM_NULLACT, MM_NULLTAG);
            exit(2);
        } if (!(o = what())) {
            fmtmsg(classification, MKDIR_LABEL, MM_ERROR, ":14:Invalid mode\n", MM_NULLACT, MM_NULLTAG);
            exit(2);
        }
        if ( (b = perms()) == ERR_PERMS) {
            fmtmsg(classification, MKDIR_LABEL, MM_ERROR, ":14:Invalid mode\n", MM_NULLACT, MM_NULLTAG);
            exit(2);
        }

        mask = m & b;

        if (o == '=')
            mode_r = mask | (mode_r & ~m);
        else if (o == '+')
            mode_r |= mask;
        else if (o == '-')
            mode_r &= ~mask;
    } while (*ms++ == ',');

    return mode_r;
}

static mode_t who() {
    register mode_t m;

    m = 0;
    for(;; ms++)
        switch(*ms) {
            case 'u':
                m |= USER;
                break;
            case 'g':
                m |= GROUP;
                break;
            case 'o':
                m |= OTHER;
                break;
        case 'a':
            m = ALL;
            break;
                case '+':
                case '=':
                case '-':
                    if (m == 0)
                        m = ALL;
                    return m;
                default:
                    return 0; /* error */
        }
    
}

static int what() 
{
    switch (*ms) {
    case '+':
    case '-':
    case '=':
        return *ms++;
    }
    return 0;
}

static mode_t perms() {
    register mode_t m;

    m = 0;
    for (;; ms++)
        switch(*ms) {
            case 'r':
                m |= READ;
                break;
            case 'w':
                m |= WRITE;
                break;
            case 'x':
                m |= EXEC;
                break;
            default:
            /* if (m == 0)
                return 0; dont change anything */
                if ((*ms == '\0') || (*ms == ','))
                    return m;
                return ERR_PERMS;
        }
}

static mode_t abs_val()
{
    register c;
    mode_t i;

    for (i = 0; (c = *ms) >= '0' && c <= '7'; ms++)
        i = (mode_t)((i << 3) + (c - '0'));
    
    if (*ms) {
        fmtmsg(classification, MKDIR_LABEL, MM_ERROR, ":14:Invalid mode\n", MM_NULLACT, MM_NULLTAG);
        exit(2);
    }
    return i;
}

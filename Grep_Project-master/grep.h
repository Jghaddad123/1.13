
enum CONST {BLKSIZE =4096,NBLK =2047, FNSIZE=128, LBSIZE=4096, ESIZE=256,GBSIZE=256,NBRA=5, CBRA=1,
    CCHR=2, CDOT=4, CCL=6, NCCL=8, CDOL=10, CEOF=11, CKET=12, CBACK=14, CCIRC=15, STAR=01, READ = 0, WRITE =1, BUFSIZE=100};

static int  vflag  = 1, tfile  = -1, iblock  = -1, oblock  = -1, regexp_index = 0, peekc, lastc, given,
        ninbuf, io, pflag, oflag, listf, listn, col, tline, ichanged, nleft, names[26], anymarks, nbra, subnewa, fchange, wrapp;
static unsigned nlall = 128;
static unsigned int  *addr1, *addr2, *dot, *dol, *zero;
static char* rbufp;
static long  count;
static char Q[] = "", T[] = "TMP", tmpXXXXX[50] = "/tmp/eXXXXX", WRERR[]  = "WRITE ERROR", line[70], *linp  = line,
        savedfile[FNSIZE], file[FNSIZE], linebuf[LBSIZE], rhsbuf[LBSIZE/2], expbuf[ESIZE+4], genbuf[LBSIZE],
        *nextip, *linebp, *globp, *genp, *tfname, *loc1, *loc2, ibuff[BLKSIZE], obuff[BLKSIZE], *braslist[NBRA],
        *braelist[NBRA], regexp_buf[BUFSIZE], * rbufp, regexp_buf[BUFSIZE];

int advance(char *lp, char *ep), append(int (*f)(void), unsigned int *a), backref(int i, char *lp), getchr(void),
        getchr_(void), putline(void), getfile(void), execute(unsigned int *addr), cclass(char *set, int c, int af);
void blkio(int b, char *buf, long (*iofcn)(int, void*, unsigned long)), compile(int eof),  error(char *s),  exfile(void)
,filename(const char* comm), global(int k), init(void), onhup(void), putchr_(int ac), putd(void), putfile(void),
        puts_(char *sp), setwide(void), setnoaddr(void), squeeze(int), readfile(const char* file), search(const char* regexp)
,regexp_buf_init(const char* regexp), ungetch_(int c), print_buf(void), reverse(char* s),
        recur(int argc, char *argv[],int i), process(char * file,char * searchterm);
char *getline_(unsigned int tl), *getblock(unsigned int atl, int iof);

int main(int argc, char *argv[]);

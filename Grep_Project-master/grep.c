#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "grep.h"
int main(int argc, char *argv[]) {
    if (argc < 3) {printf("Not enough Terms\n");return 0;}
    recur(argc,argv,2);  return 0;
}

int advance(char *lp, char *ep) {  char *curlp;  int i;
    for (;;) {
        switch (*ep++) {
            case CCHR:  if (*ep++ == *lp++) { continue; } return(0);
            case CDOT:  if (*lp++) { continue; }    return(0);
            case CDOL:  if (*lp==0) { continue; }  return(0);
            case CEOF:  loc2 = lp;  return(1);
            case CCL:   if (cclass(ep, *lp++, 1)) {  ep += *ep;  continue; }  return(0);
            case NCCL:  if (cclass(ep, *lp++, 0)) { ep += *ep;  continue; }  return(0);
            case CBRA:  braslist[*ep++] = lp;  continue;
            case CKET:  braelist[*ep++] = lp;  continue;
            case CBACK:
                if (braelist[i = *ep++] == 0) { error(Q); }
                if (backref(i, lp)) { lp += braelist[i] - braslist[i];  continue; }  return(0);
            case CBACK|STAR:
                if (braelist[i = *ep++] == 0) { error(Q); }  curlp = lp;
                while (backref(i, lp)) { lp += braelist[i] - braslist[i]; }
                while (lp >= curlp) {  if (advance(lp, ep)) { return(1); }  lp -= braelist[i] - braslist[i];  }  continue;
            case CDOT|STAR:  curlp = lp;  while (*lp++) { }                goto star;
            case CCHR|STAR:  curlp = lp;  while (*lp++ == *ep) { }  ++ep;  goto star;
            case CCL|STAR:
            case NCCL|STAR:  curlp = lp;  while (cclass(ep, *lp++, ep[-1] == (CCL|STAR))) { }  ep += *ep;  goto star;
            star:  do {  lp--;  if (advance(lp, ep)) { return(1); } } while (lp > curlp);  return(0);
            default: error(Q);
        }
    }
}
int append(int (*f)(void), unsigned int *a) {  unsigned int *a1, *a2, *rdot;  int nline, tl;  nline = 0;  dot = a;
    while ((*f)() == 0) {
        if ((dol-zero)+1 >= nlall) {  unsigned *ozero = zero;  nlall += 1024;
            if ((zero = (unsigned *)realloc((char *)zero, nlall*sizeof(unsigned)))==NULL) {  error("MEM?");  onhup();  }
            dot += zero - ozero;  dol += zero - ozero;
        }tl = putline();  nline++;  a1 = ++dol;  a2 = a1+1;  rdot = ++dot;
        while (a1 > rdot) { *--a2 = *--a1; }  *rdot = tl;
    }return(nline);
}

int backref(int i, char *lp) { char *br;br = braslist[i];
    while (*br++ == *lp++) { if (br >= braelist[i])   { return(1); } }
    return(0);
}
void blkio(int b, char *buf, long (*iofcn)(int, void*, unsigned long)) {
    lseek(tfile, (long)b*BLKSIZE, 0);
    if ((*iofcn)(tfile, buf, BLKSIZE) != BLKSIZE) {  error(T);  }
}

int cclass(char *set, int c, int af) {int n;
    if (c==0)return(0);
    n = *set++;
    while (--n){if (*set++ == c)return(af);}
    return(!af);
}
void compile(int eof) {  int c, cclcnt;  char *ep = expbuf, *lastep, bracket[NBRA], *bracketp = bracket;
    if ((c = getchr()) == '\n') { peekc = c;  c = eof; }
    if (c == eof) {  if (*ep==0) { error(Q); }  return; }
    nbra = 0;  if (c=='^') { c = getchr();  *ep++ = CCIRC; }  peekc = c;  lastep = 0;
    for (;;) {
        if (ep >= &expbuf[ESIZE]) { goto cerror; }  c = getchr();  if (c == '\n') { peekc = c;  c = eof; }
        if (c==eof) { if (bracketp != bracket) { goto cerror; }  *ep++ = CEOF;  return;  }
        if (c!='*') { lastep = ep; }
        switch (c) {
            case '\\':
                if ((c = getchr())=='(') {
                    if (nbra >= NBRA) { goto cerror; }  *bracketp++ = nbra;  *ep++ = CBRA;  *ep++ = nbra++;  continue;
                }
                if (c == ')') {  if (bracketp <= bracket) { goto cerror; }  *ep++ = CKET;  *ep++ = *--bracketp;  continue; }
                if (c>='1' && c<'1'+NBRA) { *ep++ = CBACK;  *ep++ = c-'1';  continue; }
                *ep++ = CCHR;  if (c=='\n') { goto cerror; }  *ep++ = c;  continue;
            case '.': *ep++ = CDOT;  continue;
            case '\n':  goto cerror;
            case '*':  if (lastep==0 || *lastep==CBRA || *lastep==CKET) { goto defchar; }  *lastep |= STAR; continue;
            case '$':  if ((peekc=getchr()) != eof && peekc!='\n') { goto defchar; }  *ep++ = CDOL;  continue;
            case '[':  *ep++ = CCL;  *ep++ = 0;  cclcnt = 1;  if ((c=getchr()) == '^') {  c = getchr();  ep[-2] = NCCL; }
                do {if (c=='\n') { goto cerror; }  if (c=='-' && ep[-1]!=0) {if ((c=getchr())==']') { *ep++ = '-';  cclcnt++;  break; }
                        while (ep[-1] < c) {  *ep = ep[-1] + 1;  ep++;  cclcnt++;  if (ep >= &expbuf[ESIZE]) { goto cerror; } }
                    }
                    *ep++ = c;  cclcnt++;  if (ep >= &expbuf[ESIZE]) { goto cerror; }
                } while ((c = getchr()) != ']');
                lastep[1] = cclcnt;  continue;
            defchar:  default:  *ep++ = CCHR;  *ep++ = c;
        }
    }  cerror:  expbuf[0] = 0;  nbra = 0;  error(Q);
}

void error(char *s) {  int c;  wrapp = 0;  listf = 0;  listn = 0;  putchr_('?');  puts_(s);
    count = 0;  lseek(0, (long)0, 2);  pflag = 0;  if (globp) { lastc = '\n'; }  globp = 0;  peekc = lastc;
    if(lastc) { while ((c = getchr()) != '\n' && c != EOF) { } }
    if (io > 0) { close(io);  io = -1; }
}
int execute(unsigned int *addr) {  char *p1, *p2 = expbuf;  int c;
    for (c = 0; c < NBRA; c++) {  braslist[c] = 0;  braelist[c] = 0;  }
    if (addr == (unsigned *)0) {
        if (*p2 == CCIRC) { return(0); }  p1 = loc2; } else if (addr == zero) { return(0); }
    else { p1 = getline_(*addr); }
    if (*p2 == CCIRC) {  loc1 = p1;  return(advance(p1, p2+1)); }
    if (*p2 == CCHR) {c = p2[1];
        do {  if (*p1 != c) { continue; }  if (advance(p1, p2)) {  loc1 = p1;  return(1); }
        } while (*p1++);
        return(0);
    }
    do {if (advance(p1, p2)) {  loc1 = p1;  return(1);  }  } while (*p1++);  return(0);
}
void exfile(void) {
    close(io);
    io = -1;
}

void filename(const char* comm) {
    strcpy(savedfile, comm);
    strcpy(file, comm);
}

char * getblock(unsigned int atl, int iof) {  int off, bno = (atl/(BLKSIZE/2));  off = (atl<<1) & (BLKSIZE-1) & ~03;
    if (bno >= NBLK) {  lastc = '\n';  error(T);  }  nleft = BLKSIZE - off;
    if (bno==iblock) {  ichanged |= iof;  return(ibuff+off);  }  if (bno==oblock)  { return(obuff+off);  }
    if (iof==READ) {
        if (ichanged) { blkio(iblock, ibuff, (long (*)(int, void*, unsigned long))write); }
        ichanged = 0;  iblock = bno;  blkio(bno, ibuff, read);  return(ibuff+off);
    }
    if (oblock>=0) { blkio(oblock, obuff, (long (*)(int, void*, unsigned long))write); }
    oblock = bno;  return(obuff+off);
}
int getchr(void) {  char c;
    if ((lastc=peekc)) { peekc = 0;return(lastc); }
    if (globp) { if ((lastc = *globp++) != 0) { return(lastc); }
        globp = 0;return(EOF);}
    if ((c = (char)getchr_()) <= 0) { return(lastc = EOF); }
    return lastc;
}
int getchr_() {
    char c = (regexp_index > 0) ? regexp_buf[--regexp_index] : (char)getchar();
    lastc = c&0177;
    return lastc;
}
void ungetch_(int c) {
    if (regexp_index >= BUFSIZE) {
        printf("ungetch: overflow\n");
    } else { regexp_buf[regexp_index++] = (char)c; }
}
int getfile(void) {  int c;  char *lp = linebuf, *fp = nextip;
    do {
        if (--ninbuf < 0) {
            if ((ninbuf = (int)read(io, genbuf, LBSIZE)-1) < 0) {
                if (lp>linebuf) { puts_("'\\n' appended");  *genbuf = '\n';  } else { return(EOF); }
            }
            fp = genbuf;  while(fp < &genbuf[ninbuf]) {  if (*fp++ & 0200) { break; }  }  fp = genbuf;
        }
        c = *fp++;  if (c=='\0') { continue; }
        if (c&0200 || lp >= &linebuf[LBSIZE]) {  lastc = '\n';  error(Q);  }  *lp++ = c;  count++;
    } while (c != '\n');
    *--lp = 0;  nextip = fp;  return(0);
}
char* getline_(unsigned int tl) {char *bp, *lp; int nl = nleft;lp = linebuf;bp = getblock(tl, READ);
    tl &= (unsigned int)~((BLKSIZE/2)-1);
    while ((*lp++ = *bp++)) {
        if (--nl == 0) {  bp = getblock(tl+=(BLKSIZE/2), READ);  nl = nleft;  }
    }
    return(linebuf);
}
void global(int k) {
    char *gb, globuf[BUFSIZ];
    int c;
    unsigned int *a1;
    if (globp) { error(Q); }
    setwide();
    squeeze(dol > zero);
    if ((c = getchr()) == '\n') { error(Q); }
    compile(c); regexp_index = 0;  gb = globuf;
    while ((c = getchr()) != '\n') {
        if (c == EOF) { error(Q); }
        if (c == '\\') { c = getchr();
            if (c != '\n') { *gb++ = '\\'; }
        }
        *gb++ = (char)c;
        if (gb >= &globuf[BUFSIZ-2]) { error(Q); }
    }
    if (gb == globuf) { *gb++ = 'p'; }
    *gb++ = '\n';  *gb++ = 0;
    for (a1 = zero; a1 <= dol; a1++) { *a1 &= (unsigned int)~01;
        if (a1>=addr1 && a1<=addr2 && execute(a1)==k) { *a1 |= 01; }
    }
    for (a1 = zero; a1 <= dol; a1++) {
        if (*a1 & 01) {*a1 &= (unsigned int)~01; dot = a1; globp = globuf;
            print_buf();  a1 = zero;  break;
        }
    }
}

void init(void) {  int *markp;  close(tfile);  tline = 2;
    for (markp = names; markp < &names[26]; )  {  *markp++ = 0;  }
    subnewa = 0;  anymarks = 0;  iblock = -1;  oblock = -1;  ichanged = 0;
    close(creat(tfname, 0600));  tfile = open(tfname, 2);  dot = dol = zero;
}

int match(const char *string, char *pattern)
{    int status; regex_t reg;
    if (regcomp(&reg, pattern, REG_EXTENDED|REG_NOSUB) != 0) {return(0);}
    status = regexec(&reg, string, (size_t) 0, NULL, 0);   regfree(&reg);
    if (status != 0) {return(0);}
    return(1);
}

void onhup(void) {
    signal(SIGINT, SIG_IGN);  signal(SIGHUP, SIG_IGN);
    if (dol > zero) {  addr1 = zero+1;  addr2 = dol;  io = creat("ed.hup", 0600);  if (io > 0) { putfile(); } }
    fchange = 0;
}

void process(char * file,char * searchterm){
    zero = (unsigned *) malloc(nlall * sizeof(unsigned));
    tfname = mkdtemp(tmpXXXXX);init();
    readfile(file);    search(searchterm);
}
void print_buf(){ int counter=0;
    genp = genbuf;char tempbuf[BUFSIZE];char* temp;char linebuf_[LBSIZE];char* lbufp;
    reverse(regexp_buf);
    rbufp = regexp_buf;++rbufp;
    for (temp = tempbuf; *rbufp != '\n'; rbufp++, temp++){
        *temp = *rbufp;
    }
    while (*genp != '\0') {counter++;
        for (lbufp  = linebuf_; *genp != '\n'; genp++, lbufp++){
            *lbufp = *genp;
        }
        if(match(linebuf_, tempbuf) != 0) {printf("%s line %i: %s\n",savedfile ,counter , linebuf_);
        }
        memset(linebuf_, '\0', sizeof(linebuf_));
        genp++;
    }
}
void putchr_(int ac) {  char *lp = linp;  int c = ac;
    if (listf) {
        if (c == '\n') {
            if (linp != line && linp[-1] == ' ') {
                *lp++ = '\\';
                *lp++ = 'n';
            }
        } else {
            if (col > (72 - 4 - 2)) {col = 8;*lp++ = '\\';*lp++ = '\n';*lp++ = '\t';}
            col++;
            if (c == '\b' || c == '\t' || c == '\\') {*lp++ = '\\';
                if (c == '\b') { c = 'b'; } else if (c == '\t') { c = 't'; }
                col++;
            } else if (c < ' ' || c == '\177') {*lp++ = '\\';  *lp++ = (c >> 6) + '0';  *lp++ = ((c >> 3) & 07) + '0';c = (c & 07) + '0';col += 3;}
        }
    }*lp++ = c;
    if (c == '\n' || lp >= &line[64]) {  linp = line;  write(oflag ? 2 : 1, line, lp - line);  return;  }  linp = lp;
}
void putd(void) {  int r = count % 10;  count /= 10;  if (count) { putd(); }  putchr_(r + '0');  }
void putfile(void) {unsigned int *a1;  char *fp, *lp;  int n, nib = BLKSIZE;  fp = genbuf;  a1 = addr1;
    do {
        lp = getline_(*a1++);
        for (;;) {
            if (--nib < 0) {
                n = (int)(fp-genbuf);
                if (write(io, genbuf, n) != n) {  puts_(WRERR);  error(Q);  }  nib = BLKSIZE-1;  fp = genbuf;
            }
            count++;  if ((*fp++ = *lp++) == 0) {  fp[-1] = '\n';  break; }
        }
    } while (a1 <= addr2);
    n = (int)(fp-genbuf);  if (write(io, genbuf, n) != n) {  puts_(WRERR);  error(Q); }
}
int putline(void) { char *bp, *lp;int nl= nleft;unsigned int tl=tline;fchange = 1;
    lp = linebuf;bp = getblock(tl, WRITE);tl &= ~((BLKSIZE/2)-1);
    while (*bp = *lp++) {
        if (*bp++ == '\n') {
            *--bp = 0;
            linebp = lp;
            break;
        }
        if (--nl == 0) {
            bp = getblock(tl+=(BLKSIZE/2), WRITE);
            nl = nleft;
        }
    }
    nl = tline;tline += (((lp-linebuf)+03)>>1)&077776; return(nl);
}
void puts_(char *sp) {col = 0;while (*sp)putchr_(*sp++);putchr_('\n');}

void reverse(char* s){ char* start = s; char* end = start + strlen(start) - 1;
    while (end > start) {char t = *end;*end-- = *start;*start++ = t;}
}
void readfile(const char* textfile){
    setnoaddr();
    if (vflag && fchange) { fchange = 0;  error(Q); }
    filename(textfile);
    init();
    addr2 = zero;
    if ((io = open((const char*)file, 0)) < 0) { lastc = '\n';  error(file); }
    setwide();
    squeeze(0);
    ninbuf = 0;
    append(getfile, addr2);
    exfile();
}
void recur(int argc, char *argv[],int i){
    if(i<argc) {
        process(argv[i],argv[1]);
        recur(argc,argv,++i);
    }    else return;
}
void regexp_buf_init(const char* regexp){
    strcpy(regexp_buf, regexp);
}

void search(const char* regexp){
    regexp_buf_init(regexp);
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), "/%s\n", regexp);
    const char* p = buf + strlen(buf) - 1;
    while (p >= buf) { ungetch_(*p--); }
    global(1);
}
void setwide(void) {if (!given) {addr1 = zero + (dol>zero);addr2 = dol;}}
void setnoaddr(void) {if (given)error(Q);}
void squeeze(int i) {if (addr1<zero+i || addr2>dol || addr1>addr2)error(Q);}

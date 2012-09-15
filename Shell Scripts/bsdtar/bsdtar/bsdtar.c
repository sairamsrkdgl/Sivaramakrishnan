/*  BSDTAR.C - read 4.2BSD tar tapes on System V with 14-char filename limits
 ***************************************************************************
 *	EDIT HISTORY
 *	26-Oct-89	SRMc - start writing program, using the 4.2BSD
 *				tar(4) man page and POSIX 1003.1 standard
 *			     - implement -f (specify input archive)
 *			     - implement -n (specifiy blocking factor)
 *			     - start -t (archive directory listing)
 *	27-Oct-89	SRMc - finish -t (archive directory listing)
 *			     - implement -s (swap bytes in archive)
 *			     - implement -x (extract files with automatic
 *				renaming for overlong pathname pieces
 *			     - implement -v (verbosity)
 *			     - implement filespec matching
 *	28-Oct-89	SRMc - fix the usage message to include -v and the
 *				optional file specifications
 *	19-Dec-90	SRMc - add extensive copyright comment.  ugh.
 *			     - define myfgets() as wrapper for fgets()
 *	20-Dec-90	SRMc - finish delinting as much as practical
 ***************************************************************************
 *  Copyright 1990 Summer Institute of Linguistics.  All rights reserved.
 *
 *  Permission is granted to anyone to use bsdtar for any purpose on any
 *  computer system, and to alter it and redistribute it freely, subject
 *  to the following restrictions:
 *
 *  1. The origin of bsdtar must not be misrepresented, either by explicit
 *     claim or by omission.  Since few users ever read sources, credits must
 *     appear in the documentation.
 *
 *  2. Altered versions of bsdtar must be plainly marked as such, and must not
 *     be misrepresented as being the original software.  Since few users ever
 *     read sources, credits must appear in the documentation.
 *
 *  3. bsdtar may not be sold as a commercial product.  However, a distribution
 *     fee may be charged for bsdtar, provided that recipients are made aware
 *     of their right to freely redistribute the program.
 *
 *  4. Because it is licensed free of charge, there is no warranty for bsdtar,
 *     to the extent permitted by applicable law.  The copyright holder
 *     provides bsdtar ``as is'' without warranty of any kind, either expressed
 *     or implied, including, but not limited to, the implied warranties of
 *     merchantability and fitness for a particular purpose.  The entire risk
 *     as to the quality and performance of bsdtar is with the user.  Should
 *     bsdtar prove defective, the user assumes the cost of all necessary
 *     servicing, repair or correction.  In no event, unless required by
 *     applicable law, will the copyright holder be liable to the user for any
 *     damages arising out of the use or inability to use bsdtar (including but
 *     not limited to loss of data or data being rendered inaccurate), even if
 *     such holder has been advised of the possibility of such damages.
 *
 *  5. This notice may not be removed or altered.
 */
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

extern char *ctime();
extern long atol();
extern int atoi();
extern void exit();
extern int getopt(), optind;
extern char *optarg;
extern int read(), write(), open(), close(), mkdir(), link(), chmod(), utime();
extern int stat();
extern int errno;

#define NUL '\0'

/*****************************************************************
 *  information from the 4.2BSD tar(5) man page
 */
#define TBLOCK 512
#define NBLOCK 20
#define NAMSIZ 100
union hblock
    {
    char dummy[TBLOCK];
    struct header
	{
	char name[NAMSIZ];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char linkflag;
	char linkname[NAMSIZ];
	} dbuf;
    };

/*****************************************************************
 *  information from the POSIX 1003.1 standard
 */
#define REGTYPE '0'		/* Regular file */
#define LNKTYPE '1'		/* Link */
#define SYMTYPE '2'		/* Reserved */
#define CHRTYPE '3'		/* Character special */
#define BLKTYPE '4'		/* Block special */
#define DIRTYPE '5'		/* Directory */
#define FIFOTYPE '6'		/* FIFO special */
#define CONTTYPE '7'		/* Reserved */
#define AREGTYPE '\0'		/* Regular file (compatibility for antiques) */

/*****************************************************************
 * information from utime(S) man page
 */
struct utimbuf
    {
    time_t actime;	/* file access time */
    time_t modtime;	/* file modification time */
    };

/*****************************************************************
 * NAME
 *    struct mappings
 * DESCRIPTION
 *    structure for building a linked list of filename mappings
 */
struct mappings
    {
    struct mappings *link;
    char *oldname;
    char *newname;
    };

/*****************************************************************
 *  global variables set by the command line
 */
char *progname = "";
char *infile = "/dev/mt0";	/* -f = input archive file */
int blocking = NBLOCK;		/* -n = set blocking factor */
int swapbytes = 0;		/* -s = swap bytes in archive */
int verbose = 0;		/* -v = verbosity */

int t_flag = 0;			/* -t = directory listing */
int x_flag = 0;			/* -x = extract files */

char **wantlist = (char **)NULL;	/* argv[optind] and following */
int wantlistsize;			/* optind - argc */

/*****************************************************************
 *  global for easier access
 */
int input_fd = -1;
union hblock *buffer = NULL;
unsigned buffersize = 0;
struct mappings *mappings = NULL;

char seps[] = "+=:~@";
char terms[]= "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

/***************************************************************************
 * NAME
 *    mygets
 * ARGUMENTS
 *    inbuffer - address of buffer for input
 *    size     - number of bytes in the input buffer
 * DESCRIPTION
 *    Read a string from the standard input.  If EOF, return empty string, but
 *    reopen stdin, attaching it to the terminal.
 * RETURN VALUE
 *    inbuffer
 */
char *mygets(inbuffer, size)
char *inbuffer;
int size;
{
register char *p;

if (fgets(inbuffer, size, stdin) == (char *)NULL)
    {
    if (freopen(ctermid((char *)NULL), "r", stdin) == (FILE *)NULL)
	{
	(void)fprintf(stderr,
		      "%s: EOF on standard input and cannot reopen tty\n",
		      progname );
	exit(1);
	}
    inbuffer[0] = NUL;	/* just to make sure */
    }
else if (inbuffer[0] != NUL)
    {			/* replace a trailing newline with NUL */
    p = inbuffer + strlen(inbuffer) - 1;
    if (*p == '\n')
	*p = NUL;
    }
return( inbuffer );
}

/***************************************************************************
 * NAME
 *    myalloc
 * ARGUMENTS
 *    size - number of bytes to allocate
 * DESCRIPTION
 *    "Safe" interface to malloc() -- abort the program with an error message
 *    if we run out of memory.
 * RETURN VALUE
 *    pointer to beginning of area allocated
 */
char *myalloc(size)
unsigned size;
{
register char *p;

p = (char *)malloc(size);
if (p == (char *)NULL)
    {
    (void)fprintf(stderr, "%s: Out of memory\n", progname );
    exit(1);
    }
(void)memset(p,'\0',size);
return(p);
}

/***************************************************************************
 * NAME
 *    mystrdup
 * ARGUMENTS
 *    str - pointer to character string to duplicate
 * DESCRIPTION
 *    Create a duplicate of an existing NUL-terminated character string.
 * RETURN VALUE
 *    pointer to the newly allocated and copied duplicate
 */
char *mystrdup(str)
char *str;
{
return( strcpy(myalloc((unsigned)strlen(str)+1), str) );
}

/***************************************************************************
 * NAME
 *    add_mapping
 * ARGUMENTS
 *    node - pointer to new mapping to add to the list
 * DESCRIPTION
 *    Add a filename mapping to the global list of filename mappings.
 * RETURN VALUE
 *    none
 */
void add_mapping(node)
struct mappings *node;
{
struct mappings *mp;

if (mappings == (struct mappings *)NULL)
    {
    mappings = node;
    return;
    }
for ( mp = mappings ; mp->link != (struct mappings *)NULL ; mp = mp->link )
    ;
mp->link = node;
mp->link->link = (struct mappings *)NULL;
}

/***************************************************************************
 * NAME
 *    find_old
 * ARGUMENTS
 *    name - pointer to a filename
 * DESCRIPTION
 *    Search the list of filename mappings for a matching oldname field.
 * RETURN VALUE
 *    pointer to node containing the relevant mapping, or NULL if none
 */
struct mappings *find_old(name)
char *name;
{
struct mappings *mp;

for ( mp = mappings ; mp != (struct mappings *)NULL ; mp = mp->link )
    {
    if (strcmp(mp->oldname, name) == 0)
	return( mp );
    }
return((struct mappings *)NULL);
}

/***************************************************************************
 * NAME
 *    find_new
 * ARGUMENTS
 *    name - pointer to a filename
 * DESCRIPTION
 *    Search the list of filename mappings for a matching newname field.
 * RETURN VALUE
 *    pointer to node containing the relevant mapping, or NULL if none
 */
struct mappings *find_new(name)
char *name;
{
struct mappings *mp;

for ( mp = mappings ; mp != (struct mappings *)NULL ; mp = mp->link )
    {
    if (strcmp(mp->newname, name) == 0)
	return( mp );
    }
return((struct mappings *)NULL);
}

/***************************************************************************
 * NAME
 *    setnewname
 * ARGUMENTS
 *    name - pointer to 15-byte buffer for new filename
 *    old  - pointer to old filename
 * DESCRIPTION
 *    Create a distinct filename for mapping from the old to a new filename.
 * RETURN VALUE
 *    none
 */
void setnewname(name,old)
char *name;
char *old;
{
char *p, *q;

(void)memcpy(name, old, 12);
name[14] = NUL;
for ( p = seps ; *p != NUL ; ++p )
    {
    name[12] = *p;
    for ( q = terms ; *q != NUL ; ++q )
	{
	name[13] = *q;
	if (find_new(name) == (struct mappings *)NULL)
	    return;
	}
    }
(void)fprintf(stderr,
	      "Automatic mapping for %s failed.  Enter a new name: ", old);
(void)mygets(name, 15);
}

/***************************************************************************
 * NAME
 *    fixname
 * ARGUMENTS
 *    oldname - pointer to a NUL-terminated file pathname
 *    newname - pointer to a buffer for storing a possibly revised name
 * DESCRIPTION
 *    Make sure that no components of the pathname are longer than 14
 *    characters.
 * RETURN VALUE
 *    none
 */
void fixname(oldname,newname)
char *oldname;
char *newname;
{
char *p, *q, *r;
struct mappings *mp;
size_t size;
char toolong[101];

for ( p = oldname, r = newname ; *p != NUL ; p = q )
    {
    /*
     *  get the size of this piece of the pathname (between /'s)
     */
    q = strchr(p,'/');
    if (q == (char *)NULL)
	q = p + strlen(p);
    size = q - p;
    if (size <= 14)
	{
	(void)memcpy(r, p, size);
	r += size;
	}
    else
	{
	/*
	 *  this piece is too long -- isolate in another buffer
	 */
	(void)memcpy(toolong, p, size);
	toolong[size] = NUL;
	mp = find_old(toolong);
	if (mp == (struct mappings *)NULL)
	    {
	    mp = (struct mappings *)myalloc(sizeof(struct mappings));
	    mp->oldname = mystrdup(toolong);
	    mp->newname = myalloc(15);
	    setnewname(mp->newname, mp->oldname);
	    add_mapping(mp);
	    }
	(void)strcpy(r, mp->newname);
	r += strlen(r);
	}
    if (*q == '/')
	*r++ = *q++;
    *r = NUL;
    }
if (strcmp(oldname,newname) != 0)
    {
    (void)fflush(stdout);
    (void)fprintf(stderr, "%s: renaming %s to %s\n",
					progname, oldname, newname );
    }
}

/***************************************************************************
 * NAME
 *    byteswap
 * ARGUMENTS
 *    ptr  - address of a buffer in memory
 *    size - size of the buffer in bytes
 * DESCRIPTION
 *    Swap adjacent bytes in the buffer.
 * RETURN VALUE
 *    none
 */
void byteswap(ptr,size)
char *ptr;
int size;
{
int ch;

for ( size /= 2 ; size > 0 ; --size, ++ptr )
    {
    ch = *ptr;
    *ptr = *(ptr+1);
    ++ptr;
    *ptr = ch;
    }
}

/***************************************************************************
 * NAME
 *    getblock
 * ARGUMENTS
 *    none
 * DESCRIPTION
 *    Read one block (size TBLOCK) for a tar file.
 * RETURN VALUE
 *    pointer to the block, or NULL if no more input
 */
union hblock *getblock()
{
static int num = 0;
static int k = 0;
register union hblock *hp;
/*
 *  if necessary, read the next group of blocks from the archive
 */
if (num == 0)
    {
    num = read(input_fd, (char *)buffer, buffersize);
    if (num < 0)
	{
	(void)fprintf(stderr, "%s: error reading from %s\n", progname, infile);
	exit(1);
	}
    if (num == 0)
	return( (union hblock *)NULL );
    if (swapbytes)
	byteswap(buffer, num);	/* if necessary, swap pairs of bytes */
    k = 0;
    num = (num + TBLOCK - 1) / TBLOCK;
    }
/*
 *  get the address of the next block
 *  increment the block counter and decrement the number of remaining blocks
 */
hp = &buffer[k];
++k;
--num;
return( hp );
}

/***************************************************************************
 * NAME
 *    allzero
 * ARGUMENTS
 *    hp - pointer to a TBLOCK sized block
 * DESCRIPTION
 *    Check whether or not an archive block contains only NULs.
 * RETURN VALUE
 *    1 if the block contains only NULs, 0 if it has anything else anywhere
 */
int allzero(hp)
union hblock *hp;
{
register char *p;
register int k;

for ( p = hp->dummy, k = TBLOCK ; k > 0 ; --k )
    {
    if (*p != NUL)
	return( 0 );		/* nonNUL byte */
    }
return( 1 );			/* only NUL bytes */
}

/***************************************************************************
 * NAME
 *    checksum
 * ARGUMENTS
 *    hp - pointer to a TBLOCK sized archive file header block
 * DESCRIPTION
 *    Verify the checksum of this block against the stored value.
 * RETURN VALUE
 *    nonzero if checksum is okay, zero if checksum doesn't match
 */
int checksum(hp)
union hblock *hp;
{
char tapechksum[8];
unsigned long oldsum, newsum;
unsigned char *p;
int k;
/*
 *  get the old checksum
 */
if (sscanf( hp->dbuf.chksum, "%8lo", &oldsum ) != 1)
    {
    (void)fflush(stdout);
    (void)fprintf(stderr,
		  "%s: invalid file header checksum '%c%c%c%c%c%c%c%c'\n",
		  progname,           hp->dbuf.chksum[0], hp->dbuf.chksum[1],
		  hp->dbuf.chksum[2], hp->dbuf.chksum[3], hp->dbuf.chksum[4],
		  hp->dbuf.chksum[5], hp->dbuf.chksum[6], hp->dbuf.chksum[7]);
    oldsum = 0L;
    }
(void)memcpy(tapechksum,hp->dbuf.chksum, 8);	/* save the header */
/*
 *  calculate the new checksum
 */
(void)memcpy(hp->dbuf.chksum, "        ", 8);	/* prepare for calculation */
for (newsum=0, p=(unsigned char *)(hp->dummy), k=TBLOCK ; k != 0 ; --k, ++p)
    newsum += *p;
(void)memcpy(hp->dbuf.chksum, tapechksum, 8);	/* restore the header */
/*
 *  verify that the two checksums are the same
 */
if (oldsum == newsum)
    return( 1 );
else
    {
    (void)fflush(stdout);
    (void)fprintf(stderr,
		  "%s: invalid file header checksum - %o [wanted %o]\n",
		  progname, newsum, oldsum );
    return( 0 );
    }
}

/***************************************************************************
 * NAME
 *    isdirname
 * ARGUMENTS
 *    name - pointer to name field in archive file header block
 * DESCRIPTION
 *    Check whether this filename ends with a '/', indicating a directory.
 * RETURN VALUE
 *    nonzero if directory name, zero otherwise
 */
int isdirname(name)
char *name;
{
char *p;
/*
 *  find the last character
 */
for ( p = name ; (*p != NUL) && (p < (name+100)) ; ++p )
    ;
if (p > name)
    --p;
if (*p == '/')
    return( 1 );
else
    return( 0 );
}

/***************************************************************************
 * NAME
 *    skipblocks
 * ARGUMENTS
 *    size - number of bytes in file data to skip
 * DESCRIPTION
 *    Skip over the data for a file of the given size.
 * RETURN VALUE
 *    zero if the end of the archive found, nonzero otherwise
 */
int skipblocks(size)
long size;
{
long need;
union hblock *hp;

for ( need = ((size+TBLOCK-1)/TBLOCK) ; need > 0L ; --need )
    {
    hp = getblock();
    if (hp == (union hblock *)NULL)
        return( 0 );
    }
return( 1 );
}

/***************************************************************************
 * NAME
 *    tar_t
 * ARGUMENTS
 *    name     - filename
 *    mode     - protection bits for file
 *    uid      - numeric userid of file's owner
 *    gid      - numeric groupid of file's owner
 *    size     - size of the file in bytes
 *    mtime    - time of creation of the file
 *    typeflag - file's type
 *    linkname - if LNKTYPE or SYMTYPE, name of file linked to
 * DESCRIPTION
 *    Write the file header information to stdout in human readable form.
 *    Throw away any data stored for this file.
 * RETURN VALUE
 *    none
 */
void tar_t(name, mode, uid, gid, size, mtime, typeflag, linkname)
char *name;
long mode, uid, gid, size, mtime;
unsigned char typeflag;
char *linkname;
{
char *ptime, *pyear, *pdate, *phour;

if (verbose)
    {
    ptime = ctime( &mtime );
    pdate = ptime + 4;
    ptime[10] = NUL;			/* terminate the date */
    phour = ptime + 11;
    ptime[16] = NUL;			/* terminate the time */
    pyear = ptime + 20;
    ptime[24] = NUL;			/* erase the \n */

    (void)printf("%4lo %3d/%-3d %8d %s %s %s  %s",
		mode, uid, gid, size, pdate, pyear, phour, name );
    switch (typeflag)
	{
	case REGTYPE:
	    break;		/* regular file -- no commentary needed */
	case LNKTYPE:
	    (void)printf("  (link to %s)", linkname );
	    size = 0;		/* no data in archive */
	    break;
	case SYMTYPE:
	    (void)printf("  (symbolic link to %s)", linkname );
	    size = 0;		/* no data in archive */
	    break;
	case CHRTYPE:
	    (void)printf("  (character special file)" );
	    size = 0;		/* no data in archive */
	    break;
	case BLKTYPE:
	    (void)printf("  (block special file)" );
	    size = 0;		/* no data in archive */
	    break;
	case DIRTYPE:
	    size = 0;		/* no data in archive */
	    break;
	case FIFOTYPE:
	    (void)printf("  (FIFO special file)" );
	    size = 0;		/* no data in archive */
	    break;
	case CONTTYPE:
	    (void)printf("  (high performance file)" );
	    break;
	default:
	    (void)printf("  (INVALID TYPE)" );
	    size = 0;		/* no data in archive (?) */
	    break;
	}
    (void)printf( "\n" );
    }
else
    (void)printf("%s\n", name);	/* not verbose */
/*
 *  skip over the data blocks for this file
 */
if (!skipblocks( size ))
    {
    (void)fprintf(stderr,"%s: unexpected end of archive file %s\n",
		  progname, infile );
    exit(1);
    }
}

/***************************************************************************
 * NAME
 *    wanted
 * ARGUMENTS
 *    name  - name of the output file
 * DESCRIPTION
 *    Check whether the user wants to extract this file.
 * RETURN VALUE
 *    nonzero if this file is wanted, zero if this file is unwanted
 */
int wanted(name)
char *name;
{
register int k;
size_t len;
char *p;

if (wantlistsize == 0)
    return( 1 );		/* everything unless told otherwise */
for ( k = 0 ; k < wantlistsize ; ++k )
    {
    p = wantlist[k];
    if (strcmp(name, p) == 0)
	return( 1 );		/* exact match of entire pathname */
    len = strlen(p);
    if ((strncmp(name,p,len) == 0) && (name[len] == '/'))
	return( 1 );		/* leading directory name matches */
    if ((strncmp(name,p,len) == 0) && isdirname(name))
	return( 1 );		/* leading directory name matches */
    }

return( 0 );
}

/***************************************************************************
 * NAME
 *    buildpath
 * ARGUMENTS
 *    pathname  - name of an output file
 * DESCRIPTION
 *    Try to create all leading directories in the pathname of the file.
 * RETURN VALUE
 *    none
 */
void buildpath(pathname)
char *pathname;
{
register char *p;
register int numslashes;
/*
 *  first, find out how many levels of directories we may need to create
 */
for ( numslashes = 0, p = pathname ; *p != NUL ; ++numslashes )
    {
    p = strchr(p, '/');		/* find the next '/' */
    if (p == (char *)NULL)
	break;			/* no more -- quit */
    ++p;			/* edge past this '/' */
    }
/*
 *  step through the pathname, trying to create each subdirectory in turn
 */
for ( p = pathname ; numslashes > 0 ; --numslashes )
    {
    p = strchr(p, '/');
    *p = NUL;			/* terminate leading pathname */
    if ((mkdir(pathname,0755) != 0) && (errno != EEXIST))
	{
	perror(progname);
	(void)fflush(stdout);
	(void)fprintf(stderr, "%s: cannot create directory %s\n",
		      progname, pathname );
	exit(2);
	}
    *p++ = '/';			/* restore the '/' and move on */
    }
}

/***************************************************************************
 * NAME
 *    writefile
 * ARGUMENTS
 *    name  - name of the output file
 *    mode  - protection bits for the file
 *    size  - size of the file in bytes
 *    mtime - creation date/time of the file
 * DESCRIPTION
 *    Write one output file from the archive.
 * RETURN VALUE
 *    none
 */
void writefile(name,mode,size,mtime)
char *name;
long mode;
long size;
long mtime;
{
FILE *fp;
union hblock *hp;
struct utimbuf utbuf;
struct stat filestat;
char answer[101];

while (stat(name, &filestat) == 0)
    {
    (void)fflush(stdout);
    (void)fprintf(stderr, "%s: %s already exists.  Overwrite (y/n)? [n] ",
				progname, name );
    (void)mygets(answer, 100);
    if ((answer[0] != 'y') && (answer[0] != 'Y'))
	{
	(void)fprintf(stderr, "    Replacement filename: [none] ");
	(void)mygets(answer, 100);
	name = strtok(answer, " \n\r\t\f\v");
	if (name == (char *)NULL)
	    {
	    /*
	     *  skip this file
	     */
	    if (!skipblocks( size ))
		{
		(void)fprintf(stderr,"%s: unexpected end of archive file %s\n",
					progname, infile );
		exit(1);
		}
	    return;
	    }
	}
    }

fp = fopen(name, "w");

if ((fp == (FILE *)NULL) && (errno == ENOENT))
    {
    buildpath(name);		/* try to build the desired directory path */
    fp = fopen(name,"w");
    }
if (fp == (FILE *)NULL)
    {
    perror(progname);
    (void)fflush(stdout);
    (void)fprintf(stderr, "%s: cannot create %s\n", progname, name);
    /*
     *  skip over the data blocks for this file
     */
    if (!skipblocks( size ))
	{
	(void)fprintf(stderr,"%s: unexpected end of archive file %s\n",
					progname, infile );
	exit(1);
	}
    return;
    }
/*
 *  we have an open file
 */
while (size > 0)
    {
    hp = getblock();
    if (hp == (union hblock *)NULL)
        {
	(void)fprintf(stderr,"%s: unexpected end of archive file %s\n",
					progname, infile );
	exit(1);
	}
    if (size >= TBLOCK)
	{
	if ((fwrite(hp->dummy, TBLOCK, 1, fp) != 1) || ferror(fp))
	    {
	    perror(progname);
	    (void)fflush(stdout);
	    (void)fprintf(stderr, "%s: error writing to %s\n", progname,name);
	    }
	size -= TBLOCK;
	}
    else
	{
	if ((fwrite(hp->dummy, (size_t)size, 1, fp) != 1) || ferror(fp))
	    {
	    perror(progname);
	    (void)fflush(stdout);
	    (void)fprintf(stderr, "%s: error writing to %s\n", progname,name);
	    }
	size = 0;
	}
    }
if (fclose(fp) == EOF)
    {
    perror(progname);
    (void)fflush(stdout);
    (void)fprintf(stderr, "%s: error closing %s\n", progname, name );
    }
/*
 *  restore the file creation time and file protection mode
 */
utbuf.actime  = mtime;
utbuf.modtime = mtime;
if (utime(name, &utbuf) != 0)
    {
    perror(progname);
    (void)fflush(stdout);
    (void)fprintf(stderr, "%s: cannot set modification time for %s\n",
		  progname, name);
    }
if (chmod(name, (int)mode) != 0)
    {
    perror(progname);
    (void)fflush(stdout);
    (void)fprintf(stderr, "%s: cannot set protection mode for %s\n",
		  progname, name);
    }
}

/***************************************************************************
 * NAME
 *    tar_x
 * ARGUMENTS
 *    name     - filename
 *    mode     - protection bits for file
 *    uid      - numeric userid of file's owner
 *    gid      - numeric groupid of file's owner
 *    size     - size of the file in bytes
 *    mtime    - time of creation of the file
 *    typeflag - file's type
 *    linkname - if LNKTYPE or SYMTYPE, name of file linked to
 * DESCRIPTION
 *    Extract a file from a tar archive, even if its name (or some component
 *    of its name) is longer than 14 characters.
 * RETURN VALUE
 *    none
 */
void tar_x(name, mode, uid, gid, size, mtime, typeflag, linkname)
char *name;
long mode, uid, gid, size, mtime;
unsigned char typeflag;
char *linkname;
{
char fname[101];
char lname[101];

if (!wanted(name))
    {
    /*
     *  this file not wanted, so skip over the data blocks for this file
     */
    if (!skipblocks( size ))
	{
	(void)fprintf(stderr,"%s: unexpected end of archive file %s\n",
						progname, infile );
	exit(1);
	}
    return;
    }

if (verbose)
    (void)printf("%s\n", name);
switch (typeflag)
    {
    case REGTYPE:
	fixname(name,fname);
	writefile(fname,mode,size,mtime);
	return;

    case LNKTYPE:
	fixname(name,fname);
	fixname(linkname,lname);
	if (link(lname,fname) != 0)
	    {
	    perror(progname);
	    (void)fflush(stdout);
	    (void)fprintf(stderr, "%s: cannot link %s to %s\n",
			  progname, fname, lname );
	    }
	return;

    case SYMTYPE:
	(void)fprintf(stderr, "%s: cannot create symbolic link %s to %s\n",
		      progname, name, linkname );
	break;

    case CHRTYPE:
	(void)fprintf(stderr, "%s: cannot create character special file %s\n",
		      progname, name );
	break;

    case BLKTYPE:
	(void)fprintf(stderr, "%s: cannot create block special file %s\n",
		      progname, name );
	break;

    case DIRTYPE:
	fixname(name,fname);
	if ((mkdir(fname,(int)mode) != 0) && (errno != EEXIST))
	    {
	    perror(progname);
	    (void)fflush(stdout);
	    (void)fprintf(stderr, "%s: cannot create directory %s\n",
			  progname, fname );
	    }
	return;

    case FIFOTYPE:
	(void)fprintf(stderr, "%s: cannot create FIFO file %s\n",
		      progname, name );
	break;

    case CONTTYPE:
	(void)fprintf(stderr,
		      "%s: cannot create special high performance file %s\n",
		      progname, name );
	break;

    default:
	(void)fprintf(stderr, "%s: invalid type for file %s\n",
		      progname, name );
	break;
    }
/*
 *  skip over the data blocks for this file
 */
if (!skipblocks( size ))
    {
    (void)fprintf(stderr,"%s: unexpected end of archive file %s\n",
		  progname, infile );
    exit(1);
    }
}

/***************************************************************************
 * NAME
 *    tar
 * ARGUMENTS
 *    none
 * DESCRIPTION
 *    Read a tar archive.
 * RETURN VALUE
 *    none
 */
void tar()
{
union hblock *hp;
int endblocks;
/*
 *  copied/derived from file header blocks
 */
char name[101];
long mode, uid, gid, size, mtime, chksum;
unsigned char typeflag;
char linkname[101];
/*
 *  open the archive file
 */
input_fd = open(infile, O_RDONLY);
if (input_fd < 0)
    {
    (void)fprintf(stderr, "%s: cannot open input - %s\n", progname, infile);
    exit(1);
    }
/*
 *  allocate memory for reading from the archive with unbuffered I/O
 */
buffersize = blocking * TBLOCK;
buffer = (union hblock *)myalloc( buffersize );
if (buffer == (union hblock *)NULL)
    {
    (void)fprintf(stderr, "%s: cannot allocate buffer for blocking value %d\n",
		  progname, blocking );
    exit(1);
    }
/*
 *  read the archive, parsing the file headers and skipping the rest
 */
for ( endblocks = 0 ; (hp = getblock()) != (union hblock *)NULL ; )
    {
    if (allzero(hp))			/* check for terminating marker */
	++endblocks;			/*   of two consecutive blocks of */
    else				/*   all NULs */
	endblocks = 0;
    if (endblocks == 2)
	break;				/* end of archive -- quit */
    if (endblocks)
	continue;			/* nothing in the header */
    if (!checksum(hp))
	continue;			/* invalid checksum -- try again */
    (void)memcpy( name, hp->dbuf.name, 100 );
    name[100] = NUL;			/* just to be safe */
    if (sscanf( hp->dbuf.mode, "%8lo", &mode ) != 1)
	mode = 0L;
    if (sscanf( hp->dbuf.uid,  "%8lo", &uid ) != 1)
	uid = 0L;
    if (sscanf( hp->dbuf.gid,  "%8lo", &gid ) != 1)
	gid = 0L;
    if (sscanf( hp->dbuf.size, "%12lo", &size ) != 1)
	size = 0L;
    if (sscanf( hp->dbuf.mtime, "%12lo", &mtime ) != 1)
	mtime = 0L;
    if (sscanf( hp->dbuf.chksum, "%8lo", &chksum ) != 1)
	chksum = 0L;
    if ((hp->dbuf.linkflag == AREGTYPE) && isdirname(hp->dbuf.name))
	typeflag = DIRTYPE;		/* must be from old-fashioned tar */
    else if (hp->dbuf.linkflag == AREGTYPE)
	typeflag = REGTYPE;		/* must be from old-fashioned tar */
    else
	typeflag = hp->dbuf.linkflag;
    (void)memcpy( linkname, hp->dbuf.linkname, 100 );
    linkname[100] = NUL;			/* just to be safe */

    if (t_flag)
	{
	tar_t(name, mode, uid, gid, size, mtime, typeflag, linkname);
	}
    else if (x_flag)
	{
	tar_x(name, mode, uid, gid, size, mtime, typeflag, linkname);
	}
    }

if (close(input_fd) != 0)
    {
    perror(progname);
    (void)fflush(stdout);
    (void)fprintf(stderr,"%s: error closing the input archive %s\n",
		  progname, infile );
    exit(2);
    }
}

/***************************************************************************
 * NAME
 *    main
 * ARGUMENTS
 *    argc - number of command line arguments
 *    argv - pointer to array of pointers to command line arguments
 * DESCRIPTION
 *    starting function for the BSDTAR program
 * RETURN VALUE
 *    always zero to indicate successful termination of the program
 *    [exit(nonzero) is used for aborting the program otherwise]
 */
int main(argc, argv)
int argc;
char **argv;
{
int c;
int errflg = 0;

progname = argv[0];

while ((c = getopt(argc, argv, "cf:n:stvx")) != -1)
    {
    switch (c)
	{
	case 'c':
	    (void)fprintf(stderr,
			  "Copyright 1990 Summer Institute of Linguistics\n");
	    break;
	case 'f':
	    infile = optarg;
	    break;

	case 'n':
	    if (isdigit(*optarg))
		blocking = atoi(optarg);
	    else
		{
		(void)fprintf(stderr, "%s: invalid blocking number - %s\n",
			      progname, optarg );
		++errflg;
		}
	    break;

	case 's':
	    swapbytes = 1;
	    break;

	case 'v':
	    verbose = 1;
	    break;

	case 't':
	    if (x_flag)
		++errflg;
	    else
		t_flag = 1;
	    break;

	case 'x':
	    if (t_flag)
		++errflg;
	    else
		x_flag = 1;
	    break;

	default:
	    errflg++;
	    break;
	}
    }
if (!t_flag && !x_flag)
    ++errflg;
if (errflg)
    {
    (void)fputs("\
usage: bsdtar -t|-x [-s] [-v] [-f archive] [-n blocking] [filespec(s)]\n\
    -t           tell the contents of the archive\n\
    -x           extract files\n\
    -s           swap adjacent bytes (for reading from 680x0 systems)\n\
    -v           work verbosely rather than tersely\n\
    -f archive   set the archive file (default = /dev/mt0)\n\
    -n blocking  set the blocking factor (default = 20)\n\
", stderr); (void)fputs("\
Either -t or -x is required.  The other switches are optional.\n\
After the switches, files or directories may be listed.  If given, only\n\
those items will be retrieved.  If no files or directories are specified,\n\
then bsdtar retrieves the entire archive.\n\
", stderr);
    exit(1);
    }

wantlist = &argv[optind];
wantlistsize = argc - optind;
tar();
return(0);
}

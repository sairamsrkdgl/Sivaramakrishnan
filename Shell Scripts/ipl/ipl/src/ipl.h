#include "graphic.x"
#include "ipl.d"

char Dev;				/* output device */
char D[ MAX_D_ROWS ][ MAX_D_COLS ][ DATAITEM_LEN ];	/* plot data */ 
int N_d_fields;					/* number of data fields per row */
int N_d_rows;					/* number of data rows */
char Buf[HBUFSIZ];				/* general purpose buffers */
char Buf2[512];

double Chsz;				/* character size */
double Chh;				/* height of a line of text */
double Chw;				/* character spacing */
double Chd;				/* character direction vector, x comp. */
int Paper = 0;				/* paper orientation */
double Lw;				/* current line width */
double StdLw = 1.0;			/* standard line width */
double Rgb;				/* color identifier */
double Rgbint;				/* color intensity identifier */

/* maxes for autoscaling -- currently not used */
double ASmaxes[30] = { 0.1, 0.5, 1, 5, 10, 20, 30, 50, 75, 100, 200, 300, 500, 750, 1000, 1250, 1500, 2000, 2500, 3000, 5000,
				10000, 20000, 30000, 50000, 100000 };

/* default tic increments */
double Incs[30]  = {  0.01, 0.05, 0.1, 1, 1, 2, 5, 5, 5, 10, 20, 20, 50, 50, 50, 50, 
				100, 100, 100, 200, 500, 500, 1000, 2000, 5000, 5000 };

/* default tic stub number formats */
char *Tformats[30]= { "%2.2f", "%2.2f", "%2.1f", "%2.1f", "%2.0f", "%2.0f", "%2.0f", "%2.0f", "%2.0f", "%3.0f", 
			"%3.0f", "%3.0f", "%3.0f", "%3.0f", "%4.0f", "%4.0f", "%4.0f", "%4.0f", "%4.0f", "%4.0f",
			"%4.0f", "%5.0f", "%5.0f", "%5.0f", "%5.0f", "%6.0f" };

double DXtic, DYtic;	/* tic increments */

char DXticfmt[12], DYticfmt[12];  /* printf format for numeric stubs */

char Tempfile[PATHNAME_LEN];	/* name of uniquely named temp file */

double Xmin, Xmax, Ymin, Ymax;	/* ranges of the input data, set by proc, before calling global()  */

int Hold;	/* TRUE if QuitAppend was called */

char Templatepath[PATHNAME_LEN];  /* location of templates and aux. files */

char Stdfont[FONTNAME_LEN];  /* page-wide default font */

char Controlfile[PATHNAME_LEN] = "";  /* name of control file */

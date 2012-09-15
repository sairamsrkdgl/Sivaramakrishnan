#include "graphic.x"
#include "ipl.d"

extern char Dev;
extern char D[ MAX_D_ROWS ][ MAX_D_COLS ][ DATAITEM_LEN ];	/* plot data */ 
extern int N_d_fields;					/* number of data fields per row */
extern int N_d_rows;					/* number of data rows */
extern char Buf[];					/* general purpose buffers */
extern char Buf2[];

extern double Chsz;				/* character size */
extern double Chw;				/* character spacing */
extern double Chh;				/* height of a line of text */
extern double Chd;				/* character direction vector, x comp. */
extern int Paper;				/* paper orientation */
extern double Lw;				/* current line width */
extern double StdLw;				/* standard line width */
extern double Rgb;				/* color identifier */
extern double Rgbint;				/* color intensity identifier */

extern double ASmaxes[];
extern double Incs[]; 
extern char *Tformats[];

extern double DXtic, DYtic;	
extern char DXticfmt[], DYticfmt[]; 
extern char Tempfile[];
extern double Xmin, Xmax, Ymin, Ymax;
extern int Hold;
extern char Templatepath[];
extern char Stdfont[];
extern char Controlfile[];

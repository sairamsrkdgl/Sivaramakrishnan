/* NT calls - these calls are the graphics primatives */
/* Notes:	Calls to NTtext should be always preceded by an NTmov() call.
*/
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define YES 1
#define NO 0

/* move to x, y absolute */
#define NTmov( x , y )		pcode( 'M', (double)x , (double)y, "" )

/* line to x, y absolute */
#define NTlin( x , y )		pcode( 'L', (double)x , (double)y, "" )

/* move to x, y data */
#define NTm( x , y )		pcode( 'M', da_x((double) x ) , da_y((double) y ), "" )

/* line to x, y data */
#define NTl( x , y )		pcode( 'L', da_x((double) x ) , da_y((double) y ), "" )

/* path to x, y absolute (form a polygon to be shaded later) */
#define NTpath( x , y )		pcode( 'P', (double)x , (double)y, "" )

/* path to x, y data (form a polygon to be shaded later) */
#define NTp( x , y )		pcode( 'P', da_x((double) x ) , da_y((double) y ), "" )

/* do shading, within the previously defined polygon path.. the shade can be 0 to 1 */
#define NTshade( x )		pcode( 'S', (double)x , 0.0, "" )

/* do shading, within the previously defined rectangle path.. the shade can be 0 to 1 */
#define NTrectangle( x )	pcode( 'E', (double)x, 0.0, "" ); 

/* text string s starting at the current location  */
#define NTtext( s )		pcode( 'T', 0.0, 0.0, s )

/* use font s */
#define NTfont( s )		{ if( strlen( s ) < 1 ) { pcode( 'F', 0.0, 0.0, Stdfont ); } 					else { pcode( 'F', 0.0, 0.0, s ); } }

/* use point size x, current font */
#define NTptsize( x )		{ pcode( 'I', (double)x , 0.0, "" ); Chsz = x; Chh = (x+2)/72.0; }

/* change the char direction x degrees counterclockwise */
#define NTchardir( x )		{ pcode( 'D', (double)x , 0.0, "" ); Chd = x ; }

/* center text bewteen current location and a point w ABSOLUTE units away in the current text direction */
#define NTcentext( s , w )	pcode( 'C', (double)w , 0.0, s )

/* right justify text bewteen curr loc and a point w ABSOLUTE units away in the current text direction */
#define NTrightjust( s, w )	pcode( 'J', (double)w, 0.0, s )


/* select paper orientation (0 = portrait, 1 = landscape) */
#define NTpaper( x )		{ Paper = (int) (x); pcode( 'O', (double)x , 0.0, "" ); }

/* select line attributes-- dash pattern string, line width, dash magnifier  */
#define NTlinetype( s, x, y )	{ pcode( 'Y', (double)x , (double)y, s ); Lw = x; }
/* reset line attributes to "standard" solid line */
#define NTnormline()		{ pcode( 'Y', StdLw, 1.0, "0" ); Lw = StdLw; }

/* select color */
#define NTcolor( x , y )	pcode( 'R', (double)x , (double)y, "" )

/* eject page (printers), end-of-plot (screens) */
#define NTshow()		pcode( 'Z', 0.0, 0.0, "" )

/* wait for user input */
#define NTwait()		pcode( 'W', 0.0, 0.0, "" )

/* put these around repetitive drawing operations to improve efficiency */
#define NTbatch_on()		pcode( 'B', 0.0, 0.0, "" )
#define NTbatch_off()		pcode( 'b', 0.0, 0.0, "" )

/* turn clipping to current area on/off */
#define NTclip_on()		pcode( 'K', 0.0, 0.0, "" );
#define NTclip_off()		pcode( 'k', 0.0, 0.0, "" );

/* 
Graphics notes:

- Origin is in lower left corner of "paper", regardless of orientation of paper.
- User units are inches.  All values are positive.
- Icode quads are passed to the IPL device interpreters.
- Format of i-code will be: "a x y s\n", where a is an op code, x and y
   are coordinates in inches, and s is a variable length string (may be null).
- Op codes are:		M x y	=moveto x,y
			L x y	=lineto x,y
			P x y	=pathto x,y (for defining shade area)
			S s 	=shade within path using shade s
			T 0 0 s	=text string s at current position
			F p 0 f =use font f (point size p)
			I p 0 	=use point-size p 
			D x 0	=text-direction in x degrees counter-clockwise
   			C w 0	=center text between current point for distance w
			J w 0	=right-justify text between current point for distance w
			O i	=paper orientation, i=0 portrait, i=1 landscape 
			Y w p d =line type, w = width, p = dash pattern density, d = dash pattern
			R r g b =color r,g,b (later, if ever)
			Z	=finished, show page 
		  (0 indicates "don't care" )
Notes:
- Set up for one-way communication w/printers, meaning application gets no
  feedback from printer.  This is so that the system will function properly in
  environments where printers are spooled.  Workstation drivers may, however,
  be interactive (two-way).

- Line type operator allows specification of different drawing line widths and dash
   patterns.  NTlinetype( S, X, Y) where: X = line width;  Y = dash pattern 
   magnification (0.1 to 10).  S indicates dash pattern.  If S is "0", an unbroken 
   (normal) line is produced.  If S is "1" through "8", a preset dash pattern is used.  
   Otherwise, S is assumed to hold the dash pattern string "[ n1 n2 n3.. ]".

- There should be no other NT routine calls between an NTtext call and it's preceding NTmov.  
   Set fonts and point sizes before moving to the location, then do the move, then do the text.
   There is no problem if using the current position for text (i.e. not doing a move).
*/

#define LINEAR 0
#define LOG 1
#define YYMM 2

#define WHITE 1
#define BLACK 0
extern double da_x(), da_y(), ab_x(), ab_y();


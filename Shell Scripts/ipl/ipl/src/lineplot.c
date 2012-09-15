#include "ipl.x"

Lineplot( )
{
int 	i, j, p,
	ncurves,
	xfield, yf[10],
	nlab,
	nmrk,
	nsh,
	number,
	nums,
	accum,
	stair,
	x0or1,
	stairbars;
	
double 	prvx, prvy,
	x, y,
	cx, cy,
	lblsiz,
	mrksiz,
	size,
	nofs,
	zer,
	pm[10],
	th[10],
	sh[10];

char 	lt[10][3],
	lb[10][20],
	str[12],
	mk[10][10];


/* get data fields */
gget( Buf, "Xfield" ); 
xfield = atoi( Buf );

gget( Buf, "Xstart.0or1" );
x0or1 = atoi( Buf );

gget( Buf, "Yfield" ); 
ncurves = sscanf( Buf, "%d %d %d %d %d %d %d %d %d %d", 
	&yf[0], &yf[1], &yf[2], &yf[3], &yf[4], &yf[5], &yf[6], &yf[7], &yf[8], &yf[9]  );
for( i = 0; i < ncurves; i++ ) if( yf[i] < 1 || yf[i] > N_d_fields ) { fprintf( stderr, "Yfield bad.\n" ); gdp_exit(); }
if( ncurves < 1 ) { fprintf( stderr, "Yfield must be specified.\n" ); gdp_exit(); }

gget( Buf, "Accum" ); if( Buf[0] == 'y' ) accum = YES; else accum = NO;


/* get line parameters for curves */
gget( Buf, "Linetype" );
sscanf( Buf, "%s %s %s %s %s %s %s %s %s %s", 
	lt[0],lt[1],lt[2],lt[3],lt[4],lt[5],lt[6],lt[7],lt[8],lt[9] );
gget( Buf, "Linetype.magnify" );
sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
	&pm[0],&pm[1],&pm[2],&pm[3],&pm[4],&pm[5],&pm[6],&pm[7],&pm[8],&pm[9] );
gget( Buf, "Linethick" );
sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
	&th[0],&th[1],&th[2],&th[3],&th[4],&th[5],&th[6],&th[7],&th[8],&th[9] );
gget( Buf, "Mark" );
nmrk = sscanf( Buf, "%s %s %s %s %s %s %s %s %s %s", 
	mk[0],mk[1],mk[2],mk[3],mk[4],mk[5],mk[6],mk[7],mk[8],mk[9] );
gget( Buf, "Mark.size" ); mrksiz = atof( Buf );
NTptsize( mrksiz );
size = Chh * 0.4;

gget( Buf, "Shade" );
nsh = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
	&sh[0],&sh[1],&sh[2],&sh[3],&sh[4],&sh[5],&sh[6],&sh[7],&sh[8],&sh[9] );
gget( Buf, "Zeroat" ); zer = atof( Buf );

gget( Buf, "Label" );
nlab = countln( Buf );
getln( "" );
for( i = 0; i < nlab; i++ ) {
	strcpy( lb[i], getln( Buf ) );
	}

gget( Buf, "Stairstep" ); if( Buf[0] == 'y' ) stair = YES; else stair = NO;
gget( Buf, "Stairstep.bars" ); 
if( Buf[0] == 'y' ) { stairbars = YES; stair = YES; }
else stairbars = NO; /* for steps over bars */

gget( Buf, "Label.size" ); lblsiz = atof( Buf );

gget( Buf, "Numberfinal" ); if( Buf[0] == 'y' ) number = YES; else number = NO;
gget( Buf, "Numbers" ); if( Buf[0] == 'y' ) nums = YES; else nums = NO;
gget( Buf, "Numbers.offset" ); nofs = atof( Buf );

NTptsize( lblsiz );

/* do shading */
for( j = 0; j < ncurves; j ++ ) {
	if( nsh >= j ) {
		if( stair ) fprintf( stderr, "warning, stairstep can't be combined with shading\n" ); 
		y = atof( D[0][yf[j]-1] );
		if( x0or1 ) x = 1;
		else x = atof( D[0][xfield-1] );
		NTm( x, zer );
		NTp( x, y );
		for( i = 0; i < N_d_rows-1; i++ ) {
			/* get current x, y */
			if( xfield < 1 ) x = i + 2;
			else x = atof( D[i+1][xfield-1] );
			if( accum ) y += atof( D[i+1][yf[j]-1] );
			else y = atof( D[i+1][yf[j]-1] );
			NTp( x, y );
			}
		NTp( x, zer );
		NTshade( sh[j] );
		}
	}

/* do the curves */
for( j = 0; j < ncurves; j ++ ) {


	NTlinetype( lt[j], th[j], pm[j] );

	/* find 1st y */
	y = atof( D[0][yf[j]-1] );
	/* find 1st x */
	if( x0or1 ) {
		if( stairbars ) x = 0.5;
		else x = 1;
		}
	else x = atof( D[0][xfield-1] );
	prvx = x;
	prvy = y;
	
	/* move to beginning of line */
	NTm( x, y );

	for( i = 0; i < N_d_rows-1; i++ ) {

		/* skip bad values.. */
		if( ! goodnum( D[i+1][yf[j]-1], &p ) ) {
			fprintf( stderr, "Warning, row %d field %d is bad (%s)\n", i+2, yf[j], D[i+1][yf[j]-1] );
			continue;
			}
		

		/* get current x, y */
		if( xfield < 1 ) {
			if( stairbars ) x = i+1.5;
			else x = i+2;
			}
		else 	x = atof( D[i+1][xfield-1] );
		if( accum ) y += atof( D[i+1][yf[j]-1] );
		else y = atof( D[i+1][yf[j]-1] );

		/* if doing stairsteps, get last x and y */
		if( stair ) { 
			if( xfield < 1 ) {
				if( stairbars ) prvx = i + .5 ;
				else prvx = i+1;
				}
			else prvx = atof( D[i][xfield-1] );
			if( accum ) prvy += atof( D[i][yf[j]-1] );
			else prvy = atof( D[i][yf[j]-1] );
			
			NTl( x, prvy );
			if( nums ) { 
				NTmov( da_x(prvx), da_y(prvy)+nofs );
				sprintf( str, DYticfmt, prvy );
				NTcentext( str, da_x(x)-da_x(prvx) ); 
				NTm( x, prvy ); 
				}
			}
		NTl( x, y );
		if( nums && ! stair ) { 
			sprintf( str, DYticfmt, y );
			NTmov( da_x(x)-1, da_y(y)+nofs );
			NTcentext( str, 2 ); NTm( x, y ); 
			}
		}
	
	if( stair ) { /* give line a tail */
		if( nums ) { sprintf( str, DYticfmt, y ); NTmov( da_x( x ), da_y( y )+nofs ); NTtext( str ); }
		NTm( x, y );
		x += (DXtic*.7);
		NTl( x, y );
		}
	if( nlab > 0 ) {
		NTmov( da_x( x )+ 0.05, da_y( y ) - (Chh/2) );
		NTtext( lb[j] );
		}
	if( number ) {
		sprintf( str, DYticfmt, y );
		NTmov( da_x( x )+ 0.05, da_y( y ) + (Chh/1.8) );
		NTtext( str );
		}
	/* put marks on lines */
	if( nmrk >= j ) {
		if( stair )fprintf( stderr, "warning, stairstep can't be combined with point marks\n"); 
		y = atof( D[0][yf[j]-1] );
		if( x0or1 ) x = 1;
		else x = atof( D[0][xfield-1] );
		if( strncmp( mk[j], "sym", 3 )==0 ) point( da_x( x ), da_y( y ), mk[j], size );
		for( i = 0; i < N_d_rows-1; i++ ) {
			/* get current x, y */
			if( xfield < 1 ) x = i + 2;
			else x = atof( D[i+1][xfield-1] );
			if( accum ) y += atof( D[i+1][yf[j]-1] );
			else y = atof( D[i+1][yf[j]-1] );
			if( strncmp( mk[j], "sym", 3 )==0 ) point( da_x( x ), da_y( y ), mk[j], size );
			}
		}
	}
NTnormline();
}

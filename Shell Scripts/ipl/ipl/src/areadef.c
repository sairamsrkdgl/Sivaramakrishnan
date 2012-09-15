/* Set up a graphics area and do axes, etc. */
#include "ipl.x"
#include "areas.h"

Areadef( )
{
char paper[12],
	areaname[30],
	s1[12], s2[12], s3[12], s4[12],
	uin[12];
double	xlo,
	xhi,
	ylo,
	yhi,
	dxlo,
	dxhi,
	dylo,
	dyhi;
int	n,
	areasrc,
	p,
	bad,
	loadflag,
	i;

/* see where area is coming from..  */
areasrc = 0;
gget( uin, "Area" ); if( strlen( uin ) > 0 ) areasrc = 1;
gget( uin, "Area.rectangle" ); if( strlen( uin ) > 0 ) areasrc = 2;
gget( uin, "Area.left" ); if( strlen( uin ) > 0 ) areasrc = 3;
if( areasrc < 1 ) { 
	fprintf( stderr, "Area or Area.rectangle, or Area.left+right+top+bottom must be specified.\n" ); 
	gdp_exit(); 
	}

if( areasrc == 1 ) {  /* preset area name */
	gget( areaname, "Area" );
	for( i = 0; i < Nareas; i++ ) if( strncmp( areaname, Areacoords[i], strlen( areaname )) ==0 ) break;
	if( i == Nareas ) { fprintf( stderr, "Can't find preset area '%s'.\n", areaname ); gdp_exit(); }
	if( Paper == 0 ) /* portrait */
		n = sscanf( Areacoords[i], "%*s %s %s %s %s", s1, s2, s3, s4 );
	else n = sscanf( Areacoords[i], "%*s %*s %*s %*s %*s %s %s %s %s", s1, s2, s3, s4 ); /* landscape */
	if( n != 4 ) { fprintf( stderr, "Error in area presets.\n" ); gdp_exit(); }
	xlo = atof( s1 ); ylo = atof( s2 ); xhi = atof( s3 ); yhi = atof( s4 ); 
	}
else if( areasrc == 2 ) { /* rectangle */
	gget( uin, "Area.rectangle" );
	if( strlen( uin ) > 0 ) n = sscanf( uin, "%lf %lf %lf %lf", &xlo, &ylo, &xhi, &yhi );
	if( n != 4 ) { 
		fprintf( stderr, "Area.rectangle must contain two coord pairs (low left, up right).\n" ); 
		gdp_exit(); 
		}
	}
else if( areasrc == 3 ) { /* left, right, bottom, top */
	int bad = NO;
	xlo = atof( uin );
	gget( uin, "Area.left" );if( !goodnum( uin, &p ))bad = YES; xlo = atof( uin );
	gget( uin, "Area.right" );if( !goodnum( uin, &p ))bad = YES; xhi = atof( uin );
	gget( uin, "Area.bottom" ); if( !goodnum( uin, &p ))bad = YES; ylo = atof( uin );
	gget( uin, "Area.top" ); if( !goodnum( uin, &p ))bad = YES; yhi = atof( uin );
	if( bad ) { fprintf( stderr, "Area.left+right+bottom+top incompletely specified." ); gdp_exit(); }
	}

/* scale discipline */
gget( uin, "Xscaletype" );
if( strcmp( uin, "log" )==0 ) Scale_discipline_x = LOG; 
else if( strcmp( uin, "yymm" )==0 ) Scale_discipline_x = YYMM; 
else Scale_discipline_x = LINEAR;

gget( uin, "Yscaletype" );
if( strcmp( uin, "log" )==0 ) Scale_discipline_y = LOG; 
else Scale_discipline_y = LINEAR;

/* scale bounds */
bad = NO;
gget( uin, "Xmin" ); if( ! goodnum( uin, &p )) bad = YES; dxlo = atof( uin );
gget( uin, "Xmax" ); if( ! goodnum( uin, &p )) bad = YES; dxhi = atof( uin );
gget( uin, "Ymin" ); if( ! goodnum( uin, &p )) bad = YES; dylo = atof( uin );
gget( uin, "Ymax" ); if( ! goodnum( uin, &p )) bad = YES; dyhi = atof( uin );
if( bad ) { fprintf( stderr, "Scale bounds (Xmin, Xmax, Ymin, Ymax) not all specified.\n" ); gdp_exit(); }


/* tic increment */
bad = NO;
gget( uin, "Xinc" ); if( ! goodnum( uin, &p )) bad = YES; DXtic = atof( uin );
gget( uin, "Yinc" ); if( ! goodnum( uin, &p )) bad = YES; DYtic = atof( uin );
if( bad ) { fprintf( stderr, "Tic increments incomplete.\n" ); }

/* get tic numbering format */
gget( DXticfmt, "Xticfmt" );
gget( DYticfmt, "Yticfmt" );

/* fprintf( stderr, "Scaling in x: %5.1f to %5.1f by %3.1f   in y: %5.1f to %5.1f by %3.1f\n", 
	dxlo, dxhi, DXtic, dylo, dyhi, DYtic ); */


setscale_x( xlo, xhi, dxlo, dxhi );
setscale_y( ylo, yhi, dylo, dyhi );


/* do the graphic work for axes, ticks, etc. */
areadress( );

}

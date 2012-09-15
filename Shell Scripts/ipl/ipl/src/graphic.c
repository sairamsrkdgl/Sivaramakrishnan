/* small, lowlevel routines for graphics */
#include "graphic.h"

/* linear, log, time, yymm, mmddyy, polar */

/* =========================== */
/* for setting up scaling in x */
setscale_x( xlow, xhi, datalow, datahi )
double 	xlow, 	/* absolute x location of left side of the area */
	xhi, 	/* absolute x location of the right side of the area */
	datalow, /* data-units x at the left side */
	datahi;	 /* data-units x at the right side */
{
Xlo = xlow;
Xhi = xhi;
DXlo = datalow;
DXhi = datahi;
if( datahi-datalow <= 0 || xhi-xlow <= 0 ) fprintf( stderr, "wild" );
if( Scale_discipline_x == LINEAR )Scale_x = (xhi-xlow) / (datahi-datalow) ;
else if( Scale_discipline_x == LOG )Scale_x = (xhi-xlow) / log( datahi-datalow ) ;
else if( Scale_discipline_x == YYMM ) {
	Scale_x = (xhi-xlow) / ( yymm_to_i( datahi ) - yymm_to_i( datalow ));
	DXlo = yymm_to_i( datalow );
	DXhi = yymm_to_i( datahi );
	}
}

/* =========================== */
/* for setting up scaling in y */
setscale_y( ylow, yhi, datalow, datahi )
double 	ylow, 	/* absolute y location of low side of the area */
	yhi, 	/* absolute y location of high side of the area */
	datalow, /* data-units y at the low side */
	datahi;	 /* data-units y at the high side */
{
Ylo = ylow;
Yhi = yhi;
DYlo = datalow;
DYhi = datahi;
if( datahi-datalow <= 0 || yhi-ylow <= 0 ) fprintf( stderr, "wild" );
if( Scale_discipline_y == LINEAR )Scale_y = (yhi-ylow) / (datahi-datalow) ;
else if( Scale_discipline_y == LOG )Scale_y = (yhi-ylow) / log( datahi-datalow ) ;
}

/* =========================== */
/* returns an absolute x location from a data value */
double da_x( d )
double d;
{
double f;
if( Scale_discipline_x == LINEAR ) return( Xlo + (( d - DXlo ) * Scale_x ));
else if( Scale_discipline_x == LOG ) { 
	if( d-DXlo > 0.0 )return( Xlo + ( log( d - DXlo )*Scale_x )); 
	else return( Xlo );
	}
else if( Scale_discipline_x == YYMM ) {
	if( d >= 7000 ) d = yymm_to_i( d );
	return( Xlo + (( d - DXlo) * Scale_x ));
	}
}

/* =========================== */
/* returns an absolute y location from a data value */
double da_y( d )
double d;
{
if( Scale_discipline_y == LINEAR ) return( Ylo + (( d - DYlo ) * Scale_y ));
else if( Scale_discipline_y == LOG ) {
	if( d-DYlo > 0.0 )return( Ylo + ( log( d - DYlo )*Scale_y ));
	else return( Ylo );
	}
}


/* =========================== */
/* returns a data x location from an abs value */
double ab_x( d )
double d;
{
if( Scale_discipline_x == LINEAR ) return( ( d - da_x( 0.0 ) ) / Scale_x );
else if( Scale_discipline_x == LOG ) return( exp( (d-Xlo) / Scale_x ) );
}


/* =========================== */
/* returns a data y location from an abs value */
double ab_y( d )
double d;
{
if( Scale_discipline_y == LINEAR ) return( ( d - da_y( 0.0 ) ) / Scale_y );
else if( Scale_discipline_y == LOG ) return( exp( (d-Ylo) / Scale_y ) );
}


/* ============================ */
/* Returns an integer given a YYMM date.  Jan 1970 is zero. */

yymm_to_i( m )
double m;
{
int yr, mo;

yr = (int)(m) / 100;
if( yr < 70 ) yr += 100;
mo = (int)(m) % 100;
return( ((yr-70)*12 ) + mo );
}


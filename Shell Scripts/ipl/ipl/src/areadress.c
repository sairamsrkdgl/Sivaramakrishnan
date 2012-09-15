/* areadress() - do axis scaling stubs, labels, and title */
/*  ..gets called by areadef (only) */
#include "ipl.x" 
#define LINE 1
#define SHADE 2
#define WLINE 3
#define NUMERIC 0
#define FROM_FILE 1
#define VAR 2
#define LITERAL 3


areadress( )
{
char 	str[50], 
	do_x[10], do_y[10],
	shade[12],
	month[6], year[6];
int 	i, j,
	n,
	tl, tr, 
	sl, sr, 
	grid, 
	even, 
	minor,
	stub0or1,
	varfield,
	nrows,
	no_lone_tics,
	stubisrc;
double 	tcl = 0.08, 
	mtcl = 0.035, /* tic length, major and minor */
	stop,
	xstubyofs,
	ystubxofs,
	sh,
	f, g, h;
FILE 	*fp;

/* set to uer's standard font */
gget( Buf, "Font" );
NTfont( Buf );


/* frame of the graphics area */
gget( Buf, "Frame" );
if( strcmp( Buf, "double" ) == 0 ) ab_rect( Xlo-mtcl, Ylo-mtcl, Xhi+mtcl, Yhi+mtcl, 1.0, 1 );
if( strcmp( Buf, "none" ) != 0 ) ab_rect( Xlo, Ylo, Xhi, Yhi, 1.0, 1 );
/* shade the area */
gget( Buf, "Shade" );
if( strlen( Buf ) > 0 ) {
	sh = atof( Buf );
	ab_rect( Xlo, Ylo, Xhi, Yhi, sh, 0 );
	}

gget( Buf, "Ystub.ticlen" ); tcl = atof( Buf );
gget( Buf, "Ystub.ticlen.minor" ); mtcl = atof( Buf );

/**** Y ****/
/* draw axis line */
gget( do_y, "Yaxis" ); 
if( do_y[0] == 'n' ) goto BEGIN_X; /* if 'no', skip all the y axis code */


/* stub text can be numeric, from a data field, from a file, or literally specified. */
gget( Buf, "Ystub" ); 
if( strcmp( Buf, "num" )==0 ) stubisrc = NUMERIC;
else if( Buf[0] == '@' ) { 
	stubisrc = VAR;
	varfield = atoi( &Buf[1] ); 
	if( varfield < 1 || varfield > N_d_fields ) { fprintf( stderr, "bad stub var field\n" ); gdp_exit(); }
	}
else if( strlen( Buf ) < 1 ) fprintf( stderr, "Warning, Ystub not specified.\n" );
else 	{
	stubisrc = FROM_FILE;
	fp = fopen( Buf, "r" );
	if( fp != NULL ) {
		gget( Buf, "Ystub.fileline" );
		for( i = 0; i < atoi( Buf ); i++ ) fgets( str, 30, fp );
		}
	else if( fp == NULL ) {
		sprintf( Buf2, "%s/%s", Templatepath, Buf );
		fp = fopen( Buf2, "r" );
		if( fp != NULL ) {
			gget( Buf, "Ystub.fileline" );
			for( i = 0; i < atoi( Buf ); i++ ) fgets( str, 30, fp );
			}
		else if( fp == NULL ) {
			text_tofile( Buf, Tempfile );
			fp = fopen( Tempfile, "r" );
			}
		}
	}


/* sides to put tics on */
gget( Buf, "Ystub.tics" ); 
if( smember( Buf, "left both" )) tl = YES; else tl = NO;
if( smember( Buf, "right both" )) tr = YES; else tr = NO;

/* sides to put stubs on */
gget( Buf, "Ystub.stubs" ); 
if( smember( Buf, "left both" )) sl = YES; else sl = NO;
if( smember( Buf, "right both" )) sr = YES; else sr = NO;

/* point size of stubs */
gget( Buf, "Ystub.size" ); NTptsize( atoi( Buf ) );

/* shade level for shaded blocks */
gget( shade, "Ystub.shade" );

/* grid style */
gget( Buf, "Ystub.grid" ); 
if( strcmp( Buf, "line" )==0 ) grid = LINE;
else if( strcmp( Buf, "shade" )==0 ) grid = SHADE;
else if( strcmp( Buf, "wline" )==0 ) grid = WLINE;
else grid = NO;

/* minor tics */
gget( Buf, "Ystub.minor" ); minor = atoi( Buf );

gget( Buf, "Ystub.xofs" ); ystubxofs = - atof( Buf ); /* note sign change */
gget( Buf, "Ystub.nolonetics" ); if( Buf[0] == 'y' ) no_lone_tics = 1; else no_lone_tics = 0;

/**** loop for doing y axis tics and stubs ****/
even = YES;
n = 0; /* counter for stub from var */
gget( Buf, "Ystart.0or1" ); stub0or1 = atoi( Buf );
if( stub0or1 ) { f = DYlo + (stub0or1 * DYtic); stop = DYhi - DYtic; }
else { f = DYlo; stop = DYhi; }
for( ; f <= stop +.0001; f+= DYtic ) {

	/* get stub text */
	if( stubisrc == NUMERIC )sprintf( str, DYticfmt, f );
	else if( stubisrc == VAR ) {
		if( n < N_d_rows ) strcpy( str, D[ n++ ][ atoi( &Buf[1] ) -1 ] );
		else str[0] = '\0';
		}
	else if( stubisrc == FROM_FILE) { 
		if( fgets( str, 30, fp ) == NULL ) str[0] = '\0';
		str[ strlen( str )-1 ] = '\0'; 
		if( str[0] == '!' ) { str[0] = ' '; sscanf( &str[1], "%lf", &f ); if( f > stop ) break; }
		}

	if( no_lone_tics && strlen( str ) < 1 ) { even = !even; continue; }

	if( tl ) { NTm( DXlo, f ); NTlin( da_x( DXlo ) - tcl , da_y( f ) ); } /* tics, left */
	if( tr ) { NTm( DXhi, f ); NTlin( da_x( DXhi ) + tcl , da_y( f ) ); } /* tics, right */
	if( f <= (DYhi-DYtic)+.0001 ) for( i = 0, g = f; i < minor; i++ ) {  /* minor tics */
		g+= DYtic / (double) minor ;
		if( tl ) { NTm( DXlo, g ); NTlin( da_x( DXlo ) - mtcl , da_y( g ) ); } /* tics, left */
		if( tr ) { NTm( DXhi, g ); NTlin( da_x( DXhi ) + mtcl , da_y( g ) ); } /* tics, right */
		}
	if( sl ) { NTmov( da_x( DXlo ) - 2, da_y( f ) - 0.03 ); NTrightjust( str, (2+ystubxofs) ); } /* text left */
	if( sr ) { NTmov( da_x( DXhi ) - ystubxofs , da_y( f ) - 0.03 ); NTtext( str ); }	/* text right */
	if( grid == LINE ) { 	/* lines */
		NTlinetype( "0", 0.3, 0 );
		NTm( DXlo, f ); 
		NTl( DXhi, f ); 
		NTnormline();
		}
	if( grid == WLINE ) ab_rect( da_x( DXlo ), da_y( f ) - 0.02, da_x( DXhi ), da_y( f )+ 0.02, 1.0, 0 );
	if( grid == SHADE && even && f <= (DYhi-DYtic)+.0001 ) { /* shaded blocks */
		rect( DXlo, f, DXhi, f+DYtic, atof( shade ), 0 );
		} 
	even = !even;
	}
if( stubisrc == FROM_FILE ) fclose( fp );
if( smember( do_y, "left both" ) ) { NTmov( Xlo, Ylo ); NTlin( Xlo, Yhi ); }
if( smember( do_y, "right both" ) ) { NTmov( Xhi, Ylo ); NTlin( Xhi, Yhi ); }



/**** X ****/
/* draw axis line */
BEGIN_X:
gget( do_x, "Xaxis" ); 
if( do_x[0] == 'n' ) goto TITLES; /* dont do any of this */

gget( Buf, "Xstub.ticlen" ); tcl = atof( Buf );
gget( Buf, "Xstub.ticlen.minor" ); mtcl = atof( Buf );

/* Figure out where stub text will come from.. either
 * plain numeric, from a data field, from a file, or literally specified.
 * Also available for x axis only is month/year, which actually comes from a file. */

gget( Buf, "Xstub" ); 

if( strcmp( Buf, "num" )==0 ) stubisrc = NUMERIC;
else if( Buf[0] == '@' ) { 
	stubisrc = VAR; 
	varfield = atoi( &Buf[1] ); 
	if( varfield < 1 || varfield > N_d_fields ) { fprintf( stderr, "bad stub var field\n" ); gdp_exit(); }
	}
else if( strcmp( Buf, "month" )==0 ) { 
	int start;
	if( Scale_discipline_x == YYMM ) {
		gget( Buf, "Xmin" );
		sprintf( month, "%02d", atoi( Buf ) % 100 );
		sprintf( year, "%02d", atoi( Buf ) / 100 );
		}
	else	{
		gget( month, "Xstub.startmonth" );
		gget( year, "Xstub.startyear" );
		}
	if( month[0] == '\0' || year[0] == '\0' ) {
		fprintf( stderr, "Warning, Xstub.startmonth (1-12) or Xstub.startyear (70-90) are missing.\n" );
		}
	start = (((atoi(year)%100)*12) + atoi( month )) - (70*12);
	fp = fopen( MONTHSTUB_PATH, "r" );
	if( fp == NULL ) { fprintf( stderr, "Can't open month stub file.\n" ); gdp_exit(); }
	for( i = 0; i < start; i++ ) fgets( str, 30, fp );
	stubisrc = FROM_FILE;
	}
else 	{
	stubisrc = FROM_FILE;
	fp = fopen( Buf, "r" );
	if( fp != NULL ) {
		gget( Buf, "Xstub.fileline" );
		for( i = 0; i < atoi( Buf ); i++ ) fgets( str, 30, fp );
		}
	else if( fp == NULL ) {
		sprintf( Buf2, "%s/%s", Templatepath, Buf );
		fp = fopen( Buf2, "r" );
		if( fp != NULL ) {
			gget( Buf, "Xstub.fileline" );
			for( i = 0; i < atoi( Buf ); i++ ) fgets( str, 30, fp );
			}

		else if( fp == NULL ) {
			text_tofile( Buf, Tempfile );
			fp = fopen( Tempfile, "r" );
			}
		}
	}


gget( Buf, "Xstub.tics" ); 
if( smember( Buf, "bottom both" )) tl = YES; else tl = NO;
if( smember( Buf, "top both" )) tr = YES; else tr = NO;

gget( Buf, "Xstub.stubs" ); 
if( smember( Buf, "bottom both" )) sl = YES; else sl = NO;
if( smember( Buf, "top both" )) sr = YES; else sr = NO;

gget( Buf, "Xstub.size" ); NTptsize( atoi( Buf ) );
gget( shade, "Xstub.shade" );

gget( Buf, "Xstub.grid" ); 
if( strcmp( Buf, "line" )==0 ) grid = LINE; 
else if( strcmp( Buf, "shade" )==0 ) grid = SHADE;
else if( strcmp( Buf, "wline" )==0 ) grid = WLINE;
else grid = NO;

gget( Buf, "Xstub.minor" ); minor = atoi( Buf );
gget( Buf, "Xstub.nolonetics" ); if( Buf[0] == 'y' ) no_lone_tics = 1; else no_lone_tics = 0;

/**** loop for doing x axis tics and stubs ****/
n = 0;
even = YES;
gget( Buf, "Xstub.yofs" ); xstubyofs = - atof( Buf ); /* note sign change */
gget( Buf, "Xstart.0or1" ); stub0or1 = atoi( Buf );
if( stub0or1 ) { f = DXlo + (stub0or1 * DXtic ); stop = DXhi - DXtic; }
else { f = DXlo; stop = DXhi; }
for( ; f <= stop + .0001; f+= DXtic ) {

	/* get stub text */
	if( stubisrc == NUMERIC )sprintf( str, DXticfmt, f );
	else if( stubisrc == VAR ) {
		if( n < N_d_rows ) strcpy( str, D[ n++ ][ varfield -1 ] );
		else str[0] = '\0';
		}
	else if( stubisrc == FROM_FILE) { 
		if( fgets( str, 30, fp ) == NULL ) str[0] = '\0';
		else str[ strlen( str )-1 ] = '\0'; 
		if( str[0] == '!' ) { str[0] = ' '; sscanf( &str[1], "%lf", &f ); if( f > stop ) break; }
		}

	if( no_lone_tics && strlen( str ) < 1 ) { even = !even; continue; }

	if( tl ) { NTm( f, DYlo ); NTlin( da_x( f ), da_y( DYlo ) - tcl ); }
	if( tr ) { NTm( f, DYhi ); NTlin( da_x( f ), da_y( DYhi ) + tcl ); }
	if( f <= (stop-DXtic)+.0001 ) for( i = 0, g = f; i < minor; i++ ) {  /* minor tics */
		g+= DXtic / (double) minor ;
		if( tl ) { NTm( g, DYlo ); NTlin( da_x( g ), da_y( DYlo ) - mtcl ); }
		if( tr ) { NTm( g, DYhi ); NTlin( da_x( g ), da_y( DYhi ) + mtcl ); }
		}

	if( sl ) {    /* tokens are stacked, max=3 */
		char s[3][30];
		for( i = 0; i < strlen( str ); i++ ) { 
			if( str[i] == ' ' )str[i] = '_';
			else if( str[i] == '~' ) str[i] = '\n' ;
			}
		nrows = sscanf( str, "%s %s %s", s[0], s[1], s[2] ); 
		h = da_y( DYlo ) + xstubyofs;
		for( i = 0; i < nrows; i++ ) {
			for( j = 0; j < strlen( s[i] ); j++ ) if( s[i][j]== '_' ) s[i][j]=' ';
			NTmov( da_x( f ) - 1, h ); NTcentext( s[i], 2 ); 
			h -= Chh;
			}
		}
	if( sr ){ 
		char s[3][30];
		for( i = 0; i < strlen( str ); i++ ) { 
			if( str[i] == ' ' )str[i] = '_';
			else if( str[i] == '~' ) str[i] = '\n' ;
			}
		nrows = sscanf( str, "%s %s %s", s[0], s[1], s[2] ); 
		h = -(xstubyofs) + da_y( DYhi ) + ((nrows-2)*Chh); /* */
		for( i = 0; i < nrows; i++ ) {
			for( j = 0; j < strlen( s[i] ); j++ ) if( s[i][j]== '_' ) s[i][j]=' ';
			NTmov( da_x( f ) - 1, h ); NTcentext( s[i], 2 ); 
			h -= Chh;
			}
		}


	if( grid == LINE ) { 
		NTlinetype( "0", 0.3, 0 );
		NTm( f, DYlo ); 
		NTl( f, DYhi ); 
		NTnormline();
		}
	if( grid == WLINE ) ab_rect( da_x( f ) - 0.02, da_y( DYlo ), da_x( f ) + 0.02, da_y( DYhi ), 1.0, 0 );
		
	if( grid == SHADE && even && f <= (stop-DXtic)+.0001 ) { 
		rect( f, DYlo, f+DXtic, DYhi, atof( shade ), 0 );
		} 
	even = !even;
	}
if( stubisrc == FROM_FILE ) fclose( fp );
if( smember( do_x, "top both" ) ) { NTmov( Xlo, Yhi ); NTlin( Xhi, Yhi ); }
if( smember( do_x, "bottom both" ) ){ NTmov( Xlo, Ylo ); NTlin( Xhi, Ylo ); }

TITLES:


/**** do axis identifiers ****/
	{
	int n, c1, c2, c3;
	double ofs;
	gget( Buf, "Xlabel" );
	if( strlen( Buf ) > 0 ) {
		gget( Buf2, "Xlabel.size" ); NTptsize( atof( Buf2 ) );
		gget( Buf2, "Xlabel.position" ); ofs = atof( Buf2 );
		getln( "" );
		for( i = 0; i < countln( Buf ); i++ ) {
			NTmov( Xlo, Ylo - ofs ); 
			NTcentext( getln( Buf ), (Xhi-Xlo) );
			ofs += Chh; 
			}
		}

	gget( Buf, "Ylabel" );
	if( strlen( Buf ) > 0 ) {
		gget( Buf2, "Ylabel.size" ); NTptsize( atof( Buf2 ) );
		gget( Buf2, "Ylabel.position" ); ofs = atof( Buf2 );
		NTchardir( 90 );
		getln( "" );
		for( i = 0; i < countln( Buf ); i++ ) {
			NTmov( Xlo-ofs, Ylo ); 
			NTcentext( getln( Buf ), (Yhi-Ylo) ); 
			ofs -= Chh;
			}
		NTchardir( 0 );
		}
	}

/**** area title ****/
	{
	int i = 1, c;
	double ofs;
	char just[20];

	gget( Buf, "Subtitle" );
	if( strlen( Buf ) > 0 ) {
		gget( Buf2, "Subtitle.font" ); NTfont( Buf2 );
		gget( Buf2, "Subtitle.size" ); NTptsize( atof( Buf2 ) );
		gget( Buf2, "Subtitle.above" ); ofs = atof( Buf2 );
		gget( Buf2, "Subtitle.justify" ); strcpy( just, Buf2 );

		getln( "" );
		for( i = 0; i < countln( Buf ); i++ ) {
			NTmov( Xlo+0.1, Yhi+ofs );
			if( strcmp( just, "center" )==0 ) NTcentext( getln( Buf ), Xhi-(Xlo+0.1) );
			else if( strcmp( just, "right" )==0 )NTrightjust( getln( Buf ), Xhi-(Xlo+0.1) );
			else NTtext( getln( Buf ) );
			ofs -= Chh;
			}
		}
	}
/* restore standard font */
gget( Buf, "Font" ); NTfont( Buf );
}


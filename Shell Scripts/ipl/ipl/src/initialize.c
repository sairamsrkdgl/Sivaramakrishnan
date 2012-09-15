/* IPL will be invoked by:   
			     tipl for postscript (to standard output)
			     sipl for sunview previewer
			     vipl for window-oriented composer
			     mipl for terminal-oriented composer
*/

#include "ipl.x"
#include "gdp.x"  /* for command line arguments */

static char fnm[PATHNAME_LEN];
static int filecount;

Initialize( )
{
int yr, mon, day, hr, min, sec, i;
char host[30];

sysdate( &mon, &day, &yr ); systime( &hr, &min, &sec );

strcpy( Templatepath, TEMPLATE_PATH );

sprintf( Tempfile, "%s/NTtmp%05d", INSTALL_TMP, getpid() );

Dev = Arg[0][ strlen( Arg[0] ) -4 ]; /* get output type */
Hold = 0;
if( !member( Dev, "tsvm" )) { fprintf( stderr, "Use tipl, sipl, vipl or mipl.\n" ); gdp_exit(); }

/* composers take off from here.. */
if( smember( Arg[0], "vipl" )) {
	if( Argc < 2 ) { fprintf( stderr, "A control file name argument must be given.\n" ); gdp_exit(); }
	NTinit( Dev );
	vipl_sequence( Arg[1] );
	gdp_exit();
	}

/* control file argument given.. */
if( Argc > 1 ) {
	filecount = 1;
	strcpy( fnm, Arg[1] );
	Sfp = fopen( fnm, "r" );
	}

if( Dev == 't' && Sfp == NULL ) { fprintf( stderr, "Control file not found.\n" ); exit(); }

else if( Sfp == NULL ) {
	NTinit( Dev );
	if( Dev == 's' )siplmenu( "Endoffile" );
	return( 1 );
	}

strcpy( Controlfile, fnm );	

NTinit( Dev );


DXlo = DXhi = 0; /* null out data dimensions */
DYlo = DYhi = 0;
sprintf( Buf, "%s %s (%02d%02d%02d %02d:%02d)", getlogin(), (Argc>1)?Arg[1]:"", yr, mon, day, hr, min );

strcpy( Stdfont, "/Helvetica" );
StdLw = 1.0;
NTfont( "/Helvetica" ); 
NTptsize( 6 );
NTmov( 0.1, 0.1 ); 
NTtext( Buf );

}


/* ============================================= */
/* start a new control file */
restart( s )
char s[];
{
FILE *fp;

if( Sfp != NULL ) fclose( Sfp );

if( strcmp( s, "" )==0 ) {
	filecount++;
	if( filecount >= Argc ) gdp_exit();
	strcpy( fnm, Arg[ filecount ] ); /* next file */
	}

else if( strlen( s ) > 0 ) strcpy( fnm, s );

Sfp = fopen( fnm, "r" );

if( Sfp == NULL ) {
	message( fnm, "File not found.", "", "" ); 
	return( 0 ); 
	}

strcpy( Controlfile, fnm );	
return( 1 );
}

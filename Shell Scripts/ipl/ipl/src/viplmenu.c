/* Vipl allows addition of text, arrows, etc. */

#include "ipl.x"

FILE *hfp, *ofp;
char holdingfile[80], outfile[80], dispfile[80];

vipl_sequence( cfile )
char cfile[];
{
char buf[2000], command[30], area[30];
double x, y, x2, y2;
int i, size;
FILE *fp;
char textsize[40], textfont[40], linethick[40], linedash[40], dashsize[40];
strcpy( textsize, "10" );
strcpy( textfont, "/Helvetica" );
strcpy( linethick, "0.8" );
strcpy( linedash, "0" );
strcpy( dashsize, "3" );




/* open holding file */
sprintf( holdingfile, "%s/%siplhold%05d", INSTALL_TMP, getlogin(), getpid() );
hfp = fopen( holdingfile, "w" );
if( hfp == NULL ) { fprintf( stderr, "Cannot open holding file\n" ); gdp_exit(); }

/* get dispfile name */
sprintf( dispfile, "%s/%sipldisp%05d", INSTALL_TMP, getlogin(), getpid() );

/* initialize ofp */
sprintf( outfile, "%s/%siplout%05d", INSTALL_TMP, getlogin(), getpid() );
ofp = fopen( outfile, "w" );
if( ofp == NULL ) { fprintf( stderr, "Cannot open output file\n" ); gdp_exit(); }
fclose( ofp );

libdisplay( "viplstartup.g", 1 );

fp = fopen( cfile, "r" );
if( fp == NULL ) { message( cfile, "doen't exist.", "", "Creating a new plot.." ); sleep( 1 ); }
else fclose( fp );
fp = fopen( cfile, "a" );
if( fp == NULL ) { message( "NOTE:", "This file is READ-ONLY", "", "" ); sleep( 1 ); }
else fclose( fp );

/* copy user's control file to output file to start off.. */
sprintf( buf, "cp %s %s 2> /dev/null", cfile, outfile );
system( buf );

getpick( buf, "Start", 1, 1 );
userdisplay( 1 );

while( 1 ) {

	message( "", "Main menu.", "", "" );

	/* find out the graph type.. */
	getpick( command, "Text Arrow Line Box Bullet Set_parms Dataclick Redraw Print Quit", 1, 1 );

	if( strcmp( command, "Redraw" )==0 ) userdisplay( 1 );

	else if( strcmp( command, "Dataclick" )==0 ) {
		message( "Coordinates can optionally",
			 "be saved in a file.", 
			 "Enter a file name, or",
			 "just press enter." );
		get_string_box( buf, "> ", 8.0, 9.5, 11.0, 9.8 );
		dataclick( buf );
		}

	else if( strcmp( command, "Set_parms" )==0 ) {
		while( 1 ) {
			message( "Select a parameter:", "", "The current value", " is in parentheses." );
			sprintf( buf, "1.Text_size(%s) 2.Font(%s) 3.Line_thickness(%s) 4.Line_dash(%s)\
				5.Dash_size(%s) Exit - - - -", textsize, textfont, linethick, linedash, dashsize );
			getpick( buf, buf, 1, 1 );
			if( buf[0] == '1' ) {
				message( "Select size in points..", "", "Current size is:", textsize );
				getpick( textsize, "6 7 10 11 12 14 16 18 24 -", 1, 1 );
				}
			else if( buf[0] == '2' ) { 
				message( "Select font..", "", "Current font is:", textfont );
				getpick( textfont, "fonts.mu", 1, 1 ); 
				userdisplay( 1 ); 
				}
			else if( buf[0] == '3' ) {
				libdisplay( "linethick.g" );
				message( "Click on the thickness", "of your choice..", "", "" );
				getpick( linethick, "linethick.mu", 1, 0 );
				}
			else if( buf[0] == '4' ) {
				libdisplay( "linetype.g" );
				message( "Click on the line dash", "style of your choice..", "", "" );
			 	getpick( linedash, "linetype.mu", 1, 0 );
				}
			else if( buf[0] == '5' ) {
				libdisplay( "dashsize.g" );
				message( "Click on the dash size", "of your choice..", "", "" );
			 	getpick( dashsize, "linedash.mu", 1, 0 );
				}
			else break;
			}
		userdisplay( 1 );
		}
	else if( strcmp( command, "Text" )==0 ) {
	    	while( 1 ) {
			message( "Point to place where", "text should start.", "", "" );
			get_point( &x, &y );
			message( "Enter text..", "Press control-d when done.", "", "" );
			NTptsize( atof( textsize ) );
			y -= Chh*0.4;
			get_text_box_tp( buf, "", x-0.2, y-2.0, x+4.0, y );
			fprintf( hfp, "\nProc Text:\nFont: %s\nSize: %s\nPosition: %6.2f %6.2f\nText: %s", 
				textfont, textsize, x, y, buf );
			userdisplay( 1 );
			if( commit() ) break;
			}
		}

	else if( strcmp( command, "Arrow" )==0 ) {
		while( 1 ) {
			message( "Point to place where", "arrowhead should be", "", "" );
			get_point( &x, &y );
			message( "", "", "Point to place where", "other end should be" );
			get_point( &x2, &y2 );
			fprintf( hfp, "\nProc Arrow:\nPoints: %4.2f %4.2f %4.2f %4.2f\n", x2, y2, x, y );
			userdisplay( 1 );
			if( commit() ) break;
			}
		}

	else if( strcmp( command, "Line" )==0 ) {
		while( 1 ) {
			getpick( buf, "Horizontal Vertical Unconstrained - - - - - - -", 1, 1 );
			message( "", "", "Point to place where", "one end should be" );
			get_point( &x2, &y2 );
			message( "Point to place where", "other end should be", "", "" );
			get_point( &x, &y );
			fprintf( hfp, "\nProc Draw:\nLinetype: %s\nLinethick: %s\nLinetype.magnify: %s\n",
				linedash, linethick, dashsize );
			if( buf[0] == 'U' )fprintf( hfp, "Points: %4.2f %4.2f %4.2f %4.2f\n", x2, y2, x, y );
			if( buf[0] == 'H' )fprintf( hfp, "Points: %4.2f %4.2f %4.2f %4.2f\n", x2, y2, x, y2 );
			if( buf[0] == 'V' )fprintf( hfp, "Points: %4.2f %4.2f %4.2f %4.2f\n", x2, y2, x2, y );
			userdisplay( 1 );
			if( commit() ) break;
			}
		}
			
	else if( strcmp( command, "Box" )==0 ) {
		while( 1 ) {
			message( "", "", "Point to place where", "lower-left of box should be" );
			get_point( &x2, &y2 );
			message( "Point to place where", "upper-right of box should be", "", "" );
			get_point( &x, &y );
			fprintf( hfp, "\nProc Draw:\nLinetype: %s\nLinethick: %s\nLinetype.magnify: %s\n",
				linedash, linethick, dashsize );
			fprintf( hfp, "Points: %4.2f %4.2f %4.2f %4.2f\n %4.2f %4.2f\n %4.2f %4.2f\n %4.2f %4.2f\n",
			  x2, y2, x2, y, x, y, x, y2, x2, y2 );
			userdisplay( 1 );
			if( commit() ) break;
			}
		}

	else if( strcmp( command, "Bullet" )==0 ) {
		while( 1 ) {
			message( "Point to place where", "you want the bullet", "", "" );
			get_point( &x, &y );
			fprintf( hfp, "\nProc Point:\nMark: sym6n\nPosition: %4.2f %4.2f\n", x, y );
			userdisplay( 1 );
			if( commit() ) break;
			}
		}

	else if( strcmp( command, "Print" )==0 ) {
		message( "Postscript image", "being sent to printer..", "", "" );
		sprintf( buf, "pipl %s", outfile );
		system( buf );
		}
	else if( strcmp( command, "Quit" ) ==0 ) {
		libdisplay( "clear.g", 1 );
		getpick( buf, "Resave Save_as Quit", 1, 1 );
		if( smember( buf, "Resave Save_as" )) {
			if( strcmp( buf, "Save_as" )==0 ) {
				get_string_box( dispfile, "Save as: ", 4.0, 6.0, 8.0, 6.4 );
				message( "Saved as:", dispfile, "", "" );
				sleep( 1 );
				}
			else if( strcmp( buf, "Resave" )==0 ) {
				strcpy( dispfile, cfile );
				message( dispfile, "Control file resaved.", "", "" );
				sleep( 1 );
				}
			sprintf( buf, "cp %s %s", outfile, dispfile );
			system( buf );
			}
		else gdp_exit();
		break;
		}
	}
}



/* ========================================== */
libdisplay( f, new )
char f[];
int new;
{
char fnm[80];
if( Dev == 'm' ) return( 1 );
sprintf( fnm, "%s/%s", TEMPLATE_PATH, f );
if( new )ab_rect( 0.0, 0.0, 11.0, 11.0, 1.0, 0 );
restart( fnm );
display();
}

/* ========================================== */
filedisplay( f, new )
char f[];
int new;
{
if( Dev == 'm' ) return( 1 );
if( new ) ab_rect( 0.0, 0.0, 11.0, 11.0, 1.0, 0 );
restart( f );
display();
}
/* ========================================== */
userdisplay( new )
int new;
{
char buf[150];
if( Dev == 'm' ) return( 1 );
if( hfp != NULL )fclose( hfp );
if( new ) ab_rect( 0.0, 0.0, 11.0, 11.0, 1.0, 0 );
sprintf( buf, "cat %s %s > %s", outfile, holdingfile, dispfile );
system( buf );
restart( dispfile );
display();
hfp = fopen( holdingfile, "a" );
}
/* ========================================== */
commit( prompt )
char prompt[];
{
static char buf[150];


getpick( buf, "Keep Re-do Cancel", 1, 1 );
if( strcmp( buf, "Keep" )==0 ) { 
	if( hfp != NULL )fclose( hfp );
	sprintf( buf, "cat %s >> %s", holdingfile, outfile );
	system( buf );
	hfp = fopen( holdingfile, "w" );
	return( 1 );
	}

else if( strcmp( buf, "Re-do" )==0 ) {
	if( hfp != NULL )fclose( hfp );
	hfp = fopen( holdingfile, "w" ); /* null out holding file */
	userdisplay( 1 );
	return( 0 );
	}

else if( strcmp( buf, "Cancel" )==0 ) {
	if( hfp != NULL )fclose( hfp );
	hfp = fopen( holdingfile, "w" ); /* null out holding file */
	userdisplay( 1 );
	return( 1 );
	}
}


/* usa and canada map, with state distributions expressed by shading */
#include "ipl.x"
#include "mapdata.h"
#define USA_ONLY 1
#define USA_CANADA 2
#define CANADA_ONLY 3

Map( )
{
int i, j, k, slot;
int valfield, statefield;
int nt, ns;
int start, stop;
int ctry;
double t[15], s[15];
double stab[50], ptab[12];
double sshade[50], pshade[12];
double thick, y;
FILE *fp;
char ans[10];

gget( Buf, "Country" );
if( strcmp( Buf, "usa" )==0 ) ctry = USA_ONLY;
if( strcmp( Buf, "usa+canada" )==0 ) ctry = USA_CANADA;
if( strcmp( Buf, "canada" )==0 ) ctry = CANADA_ONLY;

NTm( usa[0], usa[1] );
for( i = 0; i < nusa*2; i+=2 ) NTp( usa[i], usa[i+1] );
NTshade( 1.0 );
NTm( canada[0], canada[1] );
for( i = 0; i < ncanada*2; i+=2 ) NTp( canada[i], canada[i+1] );
NTshade( 1.0 );

gget( Buf, "Mode" ); 
if( strcmp( Buf, "country" )==0 ) goto COUNTRY_OUTLINE;
else if( strcmp( Buf, "state" )==0 ) goto STATE_OUTLINE;
/* else mode should be state-distribution */

gget( Buf, "Valfield" ); 
if( Buf[0] == '\0' ) valfield = 0;
else valfield = atoi( Buf );

gget( Buf, "Statefield" );
if( strlen( Buf ) < 1 ) { fprintf( stderr, "Statefield (containing state abreviations) is missing.\n" ); gdp_exit(); }
statefield = atoi( Buf );

/* add up data */
for( i = 0; i < nsi; i++ ) stab[i] = 0;
for( i = 0; i < npi; i++ ) ptab[i] = 0;

/* NTclip_on(); */

/* for each row of data.. */
for( i = 0; i < N_d_rows; i++ ) {

	/* check state list */
 	for( j = 0; j < nsi*2; j+=2 ) if( strcmp( D[ i ][ statefield-1 ], sindex[ j+1 ] )==0 ) break;

	if( j >= (nsi*2) ) { /* not a state, check provinces */
		for( j = 0; j < npi*2; j+=2 ) if( strcmp( D[i][ statefield-1 ], pindex[j+1 ] ) == 0 ) break;
		if( j >= (npi*2) ) { 
			fprintf( stderr, "Warning: row %d, bad state or province code (%s).\n", i, D[i][statefield-1] );
			continue;
			}
		slot = j / 2;
		/* increment counter */
		if( valfield > 0 ) ptab[ slot ] += atof( D[ i ][ valfield-1 ] );
		else if( valfield == 0 ) ptab[ slot ] += 1.0; 
		}
	else 	{
		slot = j / 2;
		/* increment counter */
		if( valfield > 0 ) stab[ slot ] += atof( D[ i ][ valfield-1 ] );
		else if( valfield == 0 ) stab[ slot ] += 1.0;
		}
	}

gget( Buf, "Threshold" ); 
nt = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&t[0],&t[1],&t[2],&t[3],&t[4],&t[5],&t[6],&t[7],&t[8],&t[9] );
gget( Buf, "Shade" );
ns = sscanf( Buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&s[0],&s[1],&s[2],&s[3],&s[4],&s[5],&s[6],&s[7],&s[8],&s[9] );

/* assign shades */
for( i = 0; i < nsi; i++ ) {
	for( j = 0; j < nt; j++ ) 
		if( stab[i] < t[j] ) { sshade[i] = s[j]; break; }
	if( j == nt ) sshade[i] = s[ns-1];
	}

for( i = 0; i < npi; i++ ) {
	for( j = 0; j < nt; j++ ) 
		if( ptab[i] < t[j] ) { pshade[i] = s[j]; break; }
	if( j == nt ) pshade[i] = s[ns-1];
	}


/* shade in states */
for( i = 0; i < nsi*2; i+=2 ) {
	start = atoi( sindex[i] )*2;
	stop = atoi( sindex[i+2] )*2;
	NTm( states[ start ], states[ start+1 ] );
	for( j = start; j < stop; j+=2 ) NTp( states[ j ], states[ j+1 ] );
	NTshade( sshade[ i / 2 ] );
	}
		
	
/* shade in provinces */
for( i = 0; i < npi*2; i+=2 ) {
	start = atoi( pindex[i] )*2;
	stop = atoi( pindex[i+2] )*2;
	NTm( provinces[ start ], provinces[ start+1 ] );
	for( j = start; j < stop; j+=2 ) NTp( provinces[ j ], provinces[ j+1 ] );
	NTshade( pshade[ i / 2 ] );
	}

/* label the blocks */
NTfont( "/Helvetica" ); NTptsize( 6 ); 
NTm( 1, 1 ); NTtext( "DC" );
NTm( 4, 1 ); NTtext( "AK" );
NTm( 7, 1 ); NTtext( "HI" );

/* do list */
gget( Buf, "List" );
if( Buf[0] == 'y' ) {
	fp = fopen( Tempfile, "w" );
	for( i = 0; i < nsi; i++ ) fprintf( fp, "%s: %3.0f\n", sname[i], stab[i] );
	for( i = 0; i < npi; i++ ) fprintf( fp, "%s: %3.0f\n", pname[i], ptab[i] );
	fclose( fp );
	sprintf( Buf, "sort %s", Tempfile );
	fp = popen( Buf, "r" );
	y = 38.8;
	NTfont( "/Helvetica" ); NTptsize( 6 ); 
	while( fgets( Buf, 100, fp ) != NULL ) {
		NTm( 40.4, y );
		NTtext( Buf );
		y-= 0.6;
		}
	fclose( fp );
	}
	

STATE_OUTLINE:
gget( Buf, "State.linethick" ); thick = atof( Buf );
NTlinetype( "0", thick, 1 );
/* outline states */
for( i = 0; i < nsi*2; i+=2 ) {
	start = atoi( sindex[i] )*2;
	stop = atoi( sindex[i+2] )*2;
	NTm( states[ start ], states[ start+1 ] );
	for( j = start; j < stop; j+=2 ) NTl( states[ j ], states[ j+1 ] );
	}
/* outline provinces */
for( i = 0; i < npi*2; i+=2 ) {
	start = atoi( pindex[i] )*2;
	stop = atoi( pindex[i+2] )*2;
	NTm( provinces[ start ], provinces[ start+1 ] );
	for( j = start; j < stop; j+=2 ) NTl( provinces[ j ], provinces[ j+1 ] );
	}

		
COUNTRY_OUTLINE:
gget( Buf, "Country.linethick" ); thick = atof( Buf );
NTlinetype( "0", thick, 1 );
NTm( usa[0], usa[1] );
for( i = 0; i < nusa*2; i+=2 ) NTl( usa[i], usa[i+1] );
NTm( canada[0], canada[1] );
for( i = 0; i < ncanada*2; i+=2 ) NTl( canada[i], canada[i+1] );


/* NTclip_off(); */
}

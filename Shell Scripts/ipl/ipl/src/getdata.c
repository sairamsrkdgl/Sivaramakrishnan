/* getdata() - Reads plot data.  
*/
#include "ipl.x"

Getdata( )
{
int i, j, n, startline, stopline, selectall, join, sf, ff, ix, append;
char	datafile[PATHNAME_LEN], selectfields[120], fillfields[120], sfstr[10], tok[80];
FILE	*datafp;

gget( Buf, "Startline" );
startline = atoi( Buf );
if( startline == 0 ) startline = 1;
gget( Buf, "Stopline" );
stopline = atoi( Buf );
if( stopline == 0 ) stopline = 9999;

gget( selectfields, "Selectfields" ); 
if( strcmp( selectfields, "all" )==0 ) selectall = 1;
else selectall = 0;

gget( Buf, "Join" ); 
if( Buf[0] == 'y' ) join = 1;
else join = 0;

gget( Buf, "Append" );
if( Buf[0] == 'y' ) append = 1;
else append = 0;

datafile[0] = '\0';
gget( Buf, "Datafile" ); 
if( strlen( Buf ) > 0 ) strcpy( datafile, Buf ); 

gget( Buf, "Data" );

if( Buf[0] != '\0' ) { /* data given in spec file-- put it in a tmp file and read it as usual */
	text_tofile( Buf, Tempfile );
	strcpy( datafile, Tempfile );
	}

else if( strlen( datafile ) < 1 ) { fprintf( stderr, "Getdata: no Data or Datafile specified.\n" ); gdp_exit(); }

/* open datafile */
if( strcmp( datafile, "-" )==0 ) { 
	datafp = stdin;
	fprintf( stderr, "Note: expecting data on stdin.\n" );
	}
else datafp = fopen( datafile, "r" );
if( datafp == NULL ) datafp = popen( datafile, "r" );
if( datafp == NULL ) { fprintf( stderr, "Cant open data source '%s'\n", datafile ); gdp_exit(); }
		
if( append ) i = N_d_rows;
else i = 0; 

j = 1;
while( fgets( Buf, 512, datafp ) != NULL ) {
	if( j < startline ) { j++; continue; }
	if( j > stopline ) break;
	if( sscanf( Buf, "%s", tok ) < 1 ) continue;
	ix = 0; sf = 0; 
	if( join ) ff = N_d_fields;
	else ff = 0;
	while( 1 ) {
		if( ff >= MAX_D_COLS ) {
			fprintf( stderr, "Warning, max of %d fields, extra fields ignored in row %d.\n", MAX_D_COLS, i );
			break;
			}
		strcpy( tok, getok( Buf, &ix ) );
		if( strlen( tok ) < 1 ) break;
		if( selectall ) { 
			sf++;
			if( strlen( tok ) > DATAITEM_LEN-1 ) { 
				fprintf( stderr, "Item too long (max= %d chars) in row %d fld %d.\n", 
					DATAITEM_LEN-1, i, sf );
				gdp_exit();
				}
			else strcpy( D[i][ff++], tok );
			}
		else 	{
			sf++;
			sprintf( sfstr, "%d", sf );
			if( smember( sfstr, selectfields ) ) {
				if( strlen( tok ) > DATAITEM_LEN-1 ) { 
					fprintf( stderr, "Item longer than %d chars (row %d fld %d).\n", 
						DATAITEM_LEN-1, i, sf );
					gdp_exit();
					}
				else strcpy( D[i][ff++], tok );
				}
			}
		}
	if( datafp == stdin && ff < 1 ) break;
	i++; j++;
	if( i >= MAX_D_ROWS ) { fprintf( stderr, "Warning: using 1st %d rows of data only.\n", MAX_D_ROWS ); break; }
	}

if( i < 1 ) { fprintf( stderr, "Plot data source (%s) is empty", datafile ); gdp_exit(); }
if( datafp == stdin ) fclose( datafp );
N_d_fields = ff;
if( join && i != N_d_rows ) { 
	fprintf( stderr, "Not enough records to join, expecting %d.\n", N_d_rows ); 
	gdp_exit();
	}
else N_d_rows = i;


/* Percents option: for the fields given, change to percents based on the field total */
gget( Buf, "Percents" );
if( strlen( Buf ) > 0 ) {
	double accum;
	int f, pf[24];
	n = sscanf( Buf, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	  &pf[0],&pf[1],&pf[2],&pf[3],&pf[4],&pf[5],&pf[6],&pf[7],&pf[8],&pf[9],&pf[10],&pf[11],&pf[12],&pf[13],&pf[14],
	  &pf[15],&pf[16],&pf[17],&pf[18],&pf[19],&pf[20],&pf[21],&pf[22],&pf[23] );
	for( i = 0; i < n; i++ ) {   /* fields */
		accum = 0;
		f = pf[i] -1;
		for( j = 0; j < N_d_rows; j++ )  /* compute row total */
			accum += atof( D[j][f] );
		if( accum == 0.0 ) continue;
		for( j = 0; j < N_d_rows; j++ )  /* compute percentages and replace */
			sprintf( D[j][f], "%f", (atof( D[j][f] ) / accum ) * 100 );
		}
	/* fprintf( stderr, "Percents have been calculated.  The new data is:\n" );
	for( i = 0; i < N_d_rows; i++ ) {
		for( j = 0; j < N_d_fields; j++ ) fprintf( stderr, "%12s ", D[i][j] );
		fprintf( stderr, "\n" );
		}
	*/
	}
}

#include "ipl.x"
/* Page-wide things, page title, etc.
   Note that user does not have to execute this.
*/
Page( )
{
int i = 1, c;
char paper[20];
double ofs;
FILE *fp;

/* paper orientation */
gget( paper, "Paperway" );
if( strcmp( paper, "landscape" )==0 ) { 
	NTpaper( 1 ); 
 	if( member( Dev, "sv" ))ab_rect( 0.0, 8.5, 11.0, 11.0, 0.5, 0 );
	}
else if( strcmp( paper, "portrait" )==0 ) { 
	NTpaper( 0 ); 
 	if( member( Dev, "sv" ))ab_rect( 8.5, 0.0, 11.0, 11.0, 0.5, 0 );
	}
else NTpaper( 0 );


/* standard line width */
gget( Buf2, "Standard.linethick" );
if( atof( Buf2 ) > 0 ) StdLw = atof( Buf2 );
NTnormline();

gget( Buf2, "Standard.font" ); strcpy( Stdfont, Buf2 );

gget( Buf2, "Title.font" ); NTfont( Buf2 );
gget( Buf2, "Title.size" ); NTptsize( atof( Buf2 ) );
gget( Buf2, "Title.belowtop" ); ofs = atof( Buf2 );

gget( Buf, "Title.file" );
if( strlen( Buf ) > 0 ) {
	fp = fopen( Buf, "r" );
	if( fp == NULL ) fp = popen( Buf, "r" );
 	if( fp == NULL ) { fprintf( stderr, "Can't open title file.\n" ); gdp_exit(); }
	while( fgets( Buf, 512, fp ) != NULL ) {
		strip_ws( Buf );
/*		if( strlen( Buf ) < 1 ) continue;		*/
		if( Paper == 0 ) NTmov( 0.0, 10.5-ofs ); 
		else NTmov( 0.0, 8-ofs );
		if( Paper == 0 && strlen( Buf ) >= 1 )NTcentext( Buf, 8 );
		if( Paper == 1 && strlen( Buf ) >= 1 )NTcentext( Buf, 10.5 );
		ofs += Chh;
		}
	fclose( fp );
	}
	

gget( Buf, "Title" );
if( strlen( Buf ) > 0 ) {
	getln( "" );
	for( i = 0; i < countln( Buf ); i++ ) {
		if( Paper == 0 ) NTmov( 0.0, 10.5-ofs ); 
		else NTmov( 0.0, 8-ofs );
		if( Paper == 0 )NTcentext( getln( Buf ), 8 );
		if( Paper == 1 )NTcentext( getln( Buf ), 10.5 );
		ofs += Chh;
		}
	}
}

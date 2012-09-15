/* constraint_check() - checks user parameter value against constraints
   from param. database, if any.  If violated, returns 0, otherwise, returns 1.

   Supported constraint types are:	example:		sample input:
	numeric range			0 to 12			2
	any number			number			3.1415
	set				{center,left,right}	left
	character size			charsize		6
	vector (x,y)			coordpair		3.55 2.00
	character string		token			hello
    *	a list of tokens sep. by space	list			0 0.2 0.4 0.7 1
    *	multi-line text			text			Hello world,
								is there anyone there?
    * No enforcement done here for these types..
*/
#include <stdio.h>
#include "../install.h"

constraint_check( value, constraint )
char value[], constraint[];
{
char token[30], val[30], cn[5][8];
double atof(), fval, high, low;
int i, j, multi, rtnval, nct, ix;

if( strlen( value ) < 1 ) return( 1 ); /* empty */

if( smember( constraint, "list text" )) return( 1 );

/* anything else should be single token (or series of single tokens with like types) */
nct = sscanf( constraint, "%*s %s %s %s %s %s", cn[0], cn[1], cn[2], cn[3], cn[4] );
multi = 0;
for( i = 0; i < nct; i++ ) if( strncmp( &cn[i][1], "list", 4 )==0 ) multi = 1;


/* check members of list.. */
rtnval = 1;
ix = 0;
while( 1 ) {
	strcpy( val, getok( value, &ix ) );
	if( val[0] == '\0' ) break;
	i = 0;

	/* sets */
	if( constraint[i] == '{' ) {
		for( j = i; j < strlen( constraint ); j++ ) if( member( constraint[j], "{,}"  )) constraint[j] = ' ';
		while( 1 ) {
			strcpy( token, getok( constraint, &i ) );
			if( token[0] == '\0' ) { 
				fprintf( stderr, "'%s' illegal.  Legal vals are: { %s }.", val, constraint ); 
				rtnval = 0;
				break;
				}
			if( strcmp( token, val )==0 ) { rtnval = 1; break; }
			}
		}
	
	/* numeric ranges */
	j = i;
	low = atof( getok( constraint, &j ) );
	if( strcmp( getok( constraint, &j ), "to" )==0 ) {
		high = atof( getok( constraint, &j ) );
		fval = atof( val );
		if( fval < low || fval > high ) 
			{ fprintf( stderr, "'%s' out of range. Range is %5.2f to %5.2f.", val, low, high ); rtnval = 0; }
		else rtnval = 1;
		}
	
	/* generic number */
	j = i;
	if( strcmp( getok( constraint, &j ), "number" ) == 0 ) {
		if( ! goodnum( val, &j ) ) 
			{ fprintf( stderr, "'%s' is not a number." , val ); rtnval = 0; }
		else rtnval = 1;
		}
	
	/* coordinate pair */
	j = i;
	if( strcmp( getok( constraint, &j ), "coordpair" ) == 0 ) {
		char val1[12], val2[12];
		sscanf( value, "%s %s", val1, val2 );
		if( !goodnum( val1, &j ) || !goodnum( val2, &j ) )
			{ fprintf( stderr, "'%s' is not a coordinate pair.", value ); rtnval = 0; }
		else rtnval = 1;
		}
	
	/* 2coordpair (rectangle) */
	j = i;
	if( strcmp( getok( constraint, &j ), "2coordpair" ) == 0 ) {
		char val1[12], val2[12], val3[12], val4[12];
		sscanf( value, "%s %s %s %s", val1, val2, val3, val4 );
		if( !goodnum( val1, &j ) || !goodnum( val2, &j ) ||
		    !goodnum( val3, &j ) || !goodnum( val4, &j ) )
			{ fprintf( stderr, "'%s' is not a 2coordpair.", value ); rtnval = 0; }
		else rtnval = 1;
		}
	
	/* char size */
	j = i;
	if( strcmp( getok( constraint, &j ), "charsize" ) == 0 ) {
		fval = atof( val );
		if( fval < 1 || fval > 36.0 ) {
			fprintf( stderr, "'%s' is not a legal char size (1 to 32).", val );
			rtnval = 0;
			}
		else rtnval = 1;
		}

	/* data field */
	j = i;
	if( strcmp( getok( constraint, &j ), "dfield" ) == 0 ) {
		if( atoi( val ) > 0 && atoi( val ) <= MAX_D_COLS ) rtnval = 1;
		else {
			fprintf( stderr, "'%s' is not a legal data field number (1 to %d).", val, MAX_D_COLS );
			rtnval = 0;
			}
		}

	/* font */
	j = i;
	if( strcmp( getok( constraint, &j ), "font" ) == 0 ) {
		if( val[0] != '/' ) {
			fprintf( stderr, "'%s': first character in font name should be '/'.", val );
			rtnval = 0;
			}
		else rtnval = 1;
		}

	/* color */
	j = i;
	if( strcmp( getok( constraint, &j ), "color" ) == 0 ) {
		if( atof( val ) < 0.0 || atof( val ) > 1.0 ) {
			fprintf( stderr, "'%s': color must be a number from 0.0 to 1.0.", val );
			rtnval = 0;
			}
		else rtnval = 1;
		}



	if( rtnval == 0 || !multi ) break;
	}

return( rtnval );
}

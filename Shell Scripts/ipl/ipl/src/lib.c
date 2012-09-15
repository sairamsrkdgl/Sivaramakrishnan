#include <stdio.h>
#include <ctype.h>


/* ================================ */
/* getfld()
/* Gets fields which are separated by ":". */
/* Natural ":" which appear in text must be prefixed by a backslash. */
/* Deletes the ':' and all leading and trailing white space. */
/* Leaves index pointing at next char. */

getfld( out, line, index )
char out[], line[];
int *index;
{
int i, j, esc;

j = 0;
for( i = *index; i < strlen( line ); i++ ) {
	if( line[i] == ' ' || line[i] == '\t' ) continue;
	else break;
	}
for( ; i < strlen( line ); i++ ) {
	if( line[i] == '\\' ) { esc = 1; continue; }
	if( line[i] == ':' && ! esc ) { i++; esc = 0; break; }
	else out[j++] = line[i];
	esc = 0;
	}
j--;
for( ; j > 0; j-- ) {
	if( out[j] == ':' || out[j] == ' ' || out[j] == '\t' || out[j] == '\n' || out[j] == '\0' ) continue;
	else break;
	}
out[ j+1 ] = '\0';
*index = i;
}


/* ==========================================
 * getok( )
*/
#define GETOKMAX	100
char *getok( string, index )
	char	string[];	/* array to obtain token from */
	int	*index;
	{
		static char	tok[GETOKMAX+1];
		register	n;
		while( member( string[(*index)], " \t\n" ) ) (*index)++; 
		/* EAT( SPACE, TAB, EOR, string, (*index) ); */
		for( n=0;
			n <= GETOKMAX &&
			string[*index] != ' '  &&
			string[*index] != '\t'  &&
			string[*index] != '\n'  &&
			string[*index] != '\0'  ;
				tok[n++] = string[(*index)++] )  ;
		tok[n] = '\0' ;
		if( n > GETOKMAX ) fprintf( stderr, "token %s too long\n", tok );
		return(tok);
	}

/* ================================== */
/* goodnum() - checks a token to see if it is a legal number,
	either float or integer, returns 1 if so, 0 if not a
	legal number.  2nd arg is precision, ie the position of
	the decimal point 
*/
#include <ctype.h>
#define YES 1
#define NO 0
goodnum( str, prec )
char str[];
int *prec;
{
int l, p, bad, point;
l = strlen( str );
if( l < 1 ) return( 0 );
bad = NO; *prec = -1;
for( p = 0; p < l; p++ ) { 
	if( str[p] == '.' ) { if( *prec == -1 ) *prec = p; else bad=YES; }
	else if( p == 0 && l > 1 && ( str[p] == '-' || str[p] == '+' ) );
	else if( ! isdigit( str[p]) ) bad=YES;
	}
if( bad ) return( 0 );
else return( 1 );
}


/* =================================== */
/* member() - returns char position if character c is a member of string s, 
		0 otherwise. Char positions start with 1 for this purpose. */
member( c, s )
char c, s[];
{
int i;
for( i = 0; i < strlen( s ); i++ ) if( s[i] == c ) return( i+1 );
return( 0 );
}



/* =================================== */
/* smember() - returns 1 if string s is present as a token in string t,
		0 otherwise. */
smember( s, t )
char s[], t[];
{
char tok[100], *getok();
int i;
i = 0;
while( 1 ) {
	strcpy( tok, getok( t, &i ) );
	if( tok[0] == '\0' ) break;
	if( strcmp( tok, s ) == 0 ) return( 1 );
	}
return( 0 );
}


/* =================================== */
/* strip white-space off of front and end of string s */
strip_ws( s )
char s[];
{
int i, j;
/* find last significant char and put a null after it */
for( j = strlen( s ) -1; j >= 0; j-- )
	if( !member( s[j], " \t\n" )) break;
s[j+1] = '\0';
/* find 1st significant char at position i */
for( i = 0; i < strlen( s ); i++ ) 
	if( !member( s[i], " \t\n" )) break; 
strcpy( s, &s[i] );
}

/* ================= */
sysdate( mon, day, yr )
int	*mon, *day, *yr ;
{
	int	tvec[2], *dtime ;

	time( tvec );
	dtime = (int *)(localtime( tvec ));
	*mon = *(dtime+4) + 1 ;
	*day = *(dtime+3)  ;
	*yr = *(dtime+5)  ;
}
/* ================= */
systime( hour, min, sec )
int	*hour, *min, *sec ;
{
	int	tvec[2], *dtime ;

	time( tvec );
	dtime = (int *)localtime( tvec );
	*hour = *(dtime+2) ;
	*min = *(dtime+1)  ;
	*sec = *(dtime)  ;
}



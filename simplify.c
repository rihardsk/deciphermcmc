
/*

File: "simplify.c"

A program for simplifying text formatting.

Copyright (c) 2008 by Jeffrey S. Rosenthal (http://probability.ca/jeff/)

Licensed for general copying, distribution and modification according to
the GNU General Public License (http://www.gnu.org/copyleft/gpl.html).

Compile with:  "cc -lm simplify.c -o simplify"
Run with:  "simplify < textfile > simplifiedfile"

*/

#include <stdio.h>
#include <string.h>

#define MAXWIDTH 72

main()
{
    int t, len, i, charsonline;
    char c, nextstring[160];

    t = 5;
    charsonline = 0;
    while ( t != EOF ) {
      fscanf(stdin, "%s", nextstring);
      len = numlett(nextstring);
/* printf("\n[THE LENGTH IS %d.]\n", len); */
      if (len > 0) {
	if ( (charsonline>0) && (charsonline+len>MAXWIDTH) ) {
	    printf("\n");
	    charsonline = len;
	} else {
	    /* printf(" "); */
	    charsonline = charsonline + len + 1;
	}
	for (i=0; i<strlen(nextstring); i++) {
	    c = nextstring[i];
	    if ( (c >= 'A') && (c <= 'Z') )
		printf("%c", c);
	    else if ( (c >= 'a') && (c <= 'z') )
		printf("%c", c + 'A' - 'a');
	    else if ( (c=='é') || (c=='É') || (c=='è') || (c=='È')
				|| (c=='ê') || (c=='Ê') || (c=='ë') )
		printf("E");
	    else if ( (c=='à') || (c=='À') || (c=='â') || (c=='Â')
				|| (c=='á') )
		printf("A");
	    else if ( (c=='ç') || (c=='Ç') )
		printf("C");
	    else if ( (c=='Ô') || (c=='ô') )
		printf("O");
	    else if ( (c=='ù') || (c=='û') )
		printf("U");
	    else if ( (c=='î') )
		printf("I");
	}
        printf(" ");
      }
      t = eatwhite();
    }
    printf("\n");
    return(0);

}

numlett(thestring)
char *thestring;
{
    int j, n, count;
    char c;

    n = strlen(thestring);
    count = 0;
    for (j=0; j<n; j++) {
	c = thestring[j];
	if ( ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) )
	    count++;
    }
    return(count);
}

eatwhite()
{
    int t;

    while ( ( (t = getc(stdin)) != EOF) && (t != '-')
			&& (t < 'a' || t > 'z') &&
                    (t < 'A' || t > 'Z') && (t < '0' || t > '9') ) 
    ;
    ungetc(t, stdin);
    return(t);
}


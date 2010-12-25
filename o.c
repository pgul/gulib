#include "gulib.h"
char *  /* -00000 becomes 000000  */
o(x) char * x;
{ int i;
if (x[0] != '-') return(x);
i=1;
while (x[i]=='0')++i;
if (x[i]==0) x[0]='0';
return(x);
}


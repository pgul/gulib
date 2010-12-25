#include "string.h"
#include "gulib.h"
char *
 sub(a,b) char *a, *b;
  { int i,j;
    char c[20];
    int sa,sb;
    sa=(a[0]=='-'?1:0);
    sb=(b[0]=='-'?1:0);
    if (sa) a[0]='0';
    if (sb) b[0]='0';
if (sa != sb )
{    add(a,b);
     if (sb) { b[0]='-'; return(o(a));};
     a[0]='-';
     return(o(a));
}
if (gt(b,a)) { i=0;
	       while(a[i]) c[i++]='0';
	       j=0;
	       while(b[j]) j++;
	       while(i>=0&&j>=0) c[i--]=b[j--];
	       sub(c,a);
	       strcpy(a,c);
	       if (sa)  b[0]='-';
		     else a[0]='-';
	       return(o(a));
	     }
    i=j=0;
    while(a[i++]); while(b[j++]);
    --j; i-=2;
    while(--j>=0)
     {
      if( (a[i--]-=(b[j]-060)) <'0' )
       { a[i+1]+=10;
         if(i<0) break;
         a[i]--;
       }
      if ( i<0 ) break;
     }
if (sa) { a[0]='-'; b[0]='-';
	}
   return(o(a));
 }


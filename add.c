#include "gulib.h"

char *
add(a,b) char *a, *b;
{ int i,j;
  int sa,sb;
  sa=(a[0]=='-'?1:0);
  sb=(b[0]=='-'?1:0);
  if (sa) a[0]='0';
  if (sb) b[0]='0';
if ( sa != sb )
{  sub(a,b);
   if (sb) { b[0]='-'; return(o(a)); }
   if (a[0]=='-') a[0]='0'; else a[0]='-';
   return(o(a));
}
  i=j=0;
  while(a[i++]); while(b[j++]);
  --j; i-=2;
  while(--j>=0)
  {
    if ( (a[i--]+=( b[j]-060)) > '9')
    { a[i+1]-=10;
    if (i<0) break;
      a[i]++;
    }
    if (i<0) break;
  }
if(sa) a[0]='-';
if(sb) b[0]='-';
 return(o(a));
}


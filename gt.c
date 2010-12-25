#include "gulib.h"
#include "string.h"
int
gt(a,b) char *a,*b;
{ int i,j;
  i=j=0;
  if(a[0]=='-')
  { if (b[0]!='-') return(0);
    return(!gt(b+1,a+1));
  }
  if(b[0]=='-') return(1);
  while(a[i]=='0') i++;
  while(b[j]=='0') j++;
  if (strlen(a)-i<strlen(b)-j)  return(0);
  else if (strlen(a)-i>strlen(b)-j) return(1);
  while(a[i]&& b[j])
  {  if (a[i] != b[j]) return(a[i]>b[j]);
     ++i;
     ++j;
  }
  if (a[i]) return(1);
  return(0);
}


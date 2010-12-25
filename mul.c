#include "string.h"
#include "gulib.h"
static void mulc(char * r,char * a,int n,int z)
{ int l; int x;
  l=0;
  while(a[l++]) r[l]='0';
  for(x=0;x<z;x++) r[l+x]='0';
  r[l+x]=0;
  r[0]='0';
  if (n==0) return;
  l-=2;
  while(l>=0)
  { x=(a[l]-060)*n;
    r[l+1] += (x % 10);
    if(r[l+1]>'9')
     { r[l+1]-=10; r[l]++;}
    r[l]   += (x / 10);
    if((r[l]>'9') && (l>0))
     { r[l]-=10; r[l-1]++; }
    if((r[l]>'9') && (l==0))
      r[l]='*';
    l--;
  }
}

char *
mul(r,s,dr,a,b) char *r, *a, *b; int s,dr;
{ int i,l; char tmpline[30],t2[30],sa,sb;
  sa=sb=0;
if (a[0]=='-') { a[0]='0'; sa=1;}
if (b[0]=='-') { b[0]='0'; sb=1;}
  for(i=0;i<s+dr;i++) t2[i]='0';
   t2[i]=0;
  i=0; l=0;
  while(b[l++]);
   l-=2;
  while(l>=0)
  {
    mulc(tmpline,a,b[l]-060,i++);
    l--;
    add(t2,tmpline);
  }
  /* укругление */
  i=t2[s];
  t2[s]=0;
  if(i>'4') add(t2,"1");
   strcpy(r,t2);
if (sa != sb) r[0]='-';
if (sa) a[0]='-';
if (sb) b[0]='-';
  return(o(r));
}


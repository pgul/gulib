#include <string.h>
#include "gulib.h"

void addspc(char * str,int skoko_vsego)
{ int i;

  for (i=strlen(str);i<skoko_vsego;i++)
    str[i]=' ';
  str[skoko_vsego]=0;
}

void stripspc(char * str)
{ int i;

  for (i=strlen(str)-1;(str[i]==' ') && (i>=0);i--);
  str[i+1]=0;
}


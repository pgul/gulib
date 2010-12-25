#include <stdio.h>
#include <stdlib.h>
#include "gulib.h"

void *(*myalloc)(long size)=_myalloc;
void (*myfree)(void *kogo)=free;

void *_myalloc(long size)
{ void *x;
  if (size==0L) size=1L;
  x=malloc(size);
  if (x!=NULL) return(x);
  exit(4);
  return(NULL);
}

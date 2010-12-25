#include <stdio.h>
#include <stdlib.h>
#include "gulib.h"

void *
myrealloc(void * block,long size)
{ void * x;
  if (size==0L) size=1L;
  x=realloc(block,size);
  if (x!=NULL) return(x);
  while(myalloc(size));
  return NULL;
}

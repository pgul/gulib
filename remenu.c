#include <stdio.h>
#include <string.h>
#include "gulib.h"

struct EVAKOITEM stack[15];
int sp=-1;

void remenu()
{ int i, j;
  chpair *otkud;

  if (sp<0) { printf("\nremenu error\n");
	      return;
	    }
  otkud=stack[sp].gde_s;
  for (i=0; i<stack[sp].sx_s; i++)
    for (j=0; j<stack[sp].sy_s; j++)
      ccell(stack[sp].x_s+i, stack[sp].y_s+j, *otkud++);
  myfree((void *)(stack[sp--].gde_s));
}

void all_remenu()
{ while (sp!= -1) remenu();
}

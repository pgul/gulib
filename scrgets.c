#include <string.h>
#include "gulib.h"

void scrgets(int x, int y, chpair *s, int skoko)
{ int maxx, maxy;
  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || x>=maxx || y>=maxy) return;
  if (y+skoko>maxy) skoko = maxy-y;
  memcpy(s, saved_scr+x*maxy+y, skoko*sizeof(*s));
}

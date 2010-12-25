#include "gulib.h"

void evakuate(int x, int y, int sx, int sy)
{ chpair *kuda;
  int i, maxx, maxy;

  graph(&maxx, &maxy, &i);
  ++sp;
  if (y+sy>maxy) sy=maxy-y;
  stack[sp].x_s=x;
  stack[sp].y_s=y;
  stack[sp].sx_s=sx;
  stack[sp].sy_s=sy;
  kuda=stack[sp].gde_s=myalloc(sx*sy*sizeof(chpair));
  for (i=0; i<sx; i++)
  { scrgets(x+i, y, kuda, sy);
    kuda+=sy;
  }
}

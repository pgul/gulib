#include "gulib.h"

void barfill(chtype cv, int x, int y, int sx, int sy)
{ sx+=x;
  for(; x<sx; x++)
    fill(cv, x, y, sy);
}

void barcell(chpair cell, int x, int y, int sx, int sy)
{ sx+=x;
  for(; x<sx; x++)
    putcell(cell, x, y, sy);
}

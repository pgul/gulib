#include "gulib.h"

void barputcol(chtype cv, int x, int y, int sx, int sy)
{ sx+=x;
  for(; x<sx; x++)
    putcol(cv, x, y, sy);
}

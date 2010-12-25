#include "gulib.h"
#include "keyc.h"

chtype   ramka_shade=blackf+white;

int mramka(chtype cv, int x, int y, int sx, int sy)
{
  int maxx, maxy, col;

  sx+=2;
  sy+=2;
  graph(&maxx, &maxy, &col);
  if (x<0) x=0;
  if (y<0) y=0;
  if (sx<=0) sx=1;
  if (sy<=0) sy=1;
  if (y>=maxy) y=maxy-1;
  if (y+sy>maxy) sy=maxy-y;
  if (x>=maxx) x=maxx-1;
  if (x+sx>maxx) sx=maxx-x;
  barputcol(cv, x, y, sx, sy);
  cchar(x, y, acs.tl);
  fill(acs.hor, x, y+1, sy-2);
  cchar(x, y+sy-1, acs.tr);
  sx+=x;
  --sx;
  for (++x; x<sx; x++)
  { cchar(x, y, acs.ver);
    fill(' ', x, y+1, sy-2);
    cchar(x, y+sy-1, acs.ver);
    if (y+sy+2<=maxy)
      putcol(ramka_shade, x, y+sy, 2);
  }
  cchar(x, y, acs.bl);
  fill(acs.hor, x, y+1, sy-2);
  cchar(x, y+sy-1, acs.br);
  if (y+sy+2<=maxy)
  { putcol(ramka_shade, x, y+sy, 2);
    sy-=2;
  }
  if (y+3<maxy)
    putcol(ramka_shade, x+1, y+2, sy);
  return 0;
}

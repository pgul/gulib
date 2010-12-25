#include <string.h>
#include "gulib.h"

extern int updated_scr;

static int cur_x, cur_y;

void gulib_sem(int set);

void cline (int x, int y, char *line)
{ cnline(x, y, line, strlen(line));
}

void cnline(int x, int y, char *line, int skoko)
{ 
  int maxx, maxy, i;

  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || skoko<=0 || x>=maxx || y>=maxy) return;
  if (y+skoko > maxy) skoko=maxy-y;
  for (i=0; i<skoko && line[i]; i++)
  {
    saved_scr[maxy*x+y+i].ch = (chtype)line[i];
  }
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

void cchar(int x, int y, chtype ch)
{ 
  int maxx, maxy;

  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || x>=maxx || y>=maxy) return;
  saved_scr[maxy*x+y].ch = ch;
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

void ccell(int x, int y, chpair cell)
{ 
  putcell(cell, x, y, 1);
#ifdef DEBUG
  g_refresh();
#endif
}

void putcol(chtype cv, int x, int y, int skoko)
{
  int maxx, maxy, i;

  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || skoko<=0 || x>=maxx || y>=maxy) return;
  if (y+skoko > maxy) skoko=maxy-y;
  for (i=0; i<skoko; i++)
    saved_scr[maxy*x+y+i].att = cv;
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

void fill(chtype ch, int x, int y, int skoko)
{
  int maxx, maxy, i;

  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || skoko<=0 || x>=maxx || y>=maxy) return;
  if (y+skoko > maxy) skoko=maxy-y;
  for (i=0; i<skoko; i++)
    saved_scr[maxy*x+y+i].ch = ch;
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

void putcell(chpair cell, int x, int y, int skoko)
{
  int maxx, maxy, i;

  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || skoko<=0 || x>=maxx || y>=maxy) return;
  if (y+skoko > maxy) skoko=maxy-y;
  for (i=0; i<skoko; i++)
  { saved_scr[maxy*x+y+i].ch  = cell.ch;
    saved_scr[maxy*x+y+i].att = cell.att;
  }
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

void place_nolock(int x, int y)
{ 
  int maxx, maxy;

  graph(&maxx, &maxy, NULL);
  if (x<0 || y<0 || x>=maxx || y>=maxy) return;
  /* move(x, y); */
  cur_x = x;
  cur_y = y;
}

void place(int x, int y)
{ 
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  place_nolock(x, y);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
}

int curx(void)
{
  return cur_x;
}

int cury(void)
{
  return cur_y;
}

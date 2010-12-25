#include "gulib.h"
#include "keyc.h"

#define SCROLLPAGE
#define MAXSCRLSTACK 10
#define _scrollbarc      ((0xBA<<8)|0x2C)
#define _scrollbarupc    ((0xBA<<8)|0x2D)
#define _scrollbardownc  ((0xBA<<8)|0x2E)
#define _scrollbarpgupc  ((0xBA<<8)|0x2F)
#define _scrollbarpgdnc  ((0xBA<<8)|0x30)
#define dnbar            (upbar+barsize)

void back_key_first(int key);

chtype bottcol=whitef+black;
chtype scrollcol=0; /* do not change */
static struct
       { int x, y, h, vsego, tek; } sstack[MAXSCRLSTACK];
static int sptr=-1;

static int  moving=0, gde, gde_last, barsize, upbar;
static void scrollbarf(void);
static void scrollbarup(void);
static void scrollbardown(void);
static void scrollbarpgup(void);
static void scrollbarpgdn(void);
static void scrollmove(void);

#define x            sstack[sptr].x
#define y            sstack[sptr].y
#define h            sstack[sptr].h
#define vsego        sstack[sptr].vsego
#define tek          sstack[sptr].tek

void drawscroll(int x_, int y_, int h_, int vsego_, int tek_)
{ int i;

  if (++sptr>=MAXSCRLSTACK)
    return;
  if (sptr==0) {
    onkey(_scrollbarc, scrollbarf);
    onkey(_scrollbarupc, scrollbarup);
    onkey(_scrollbardownc, scrollbardown);
    onkey(_scrollbarpgupc, scrollbarpgup);
    onkey(_scrollbarpgdnc, scrollbarpgdn);
  }
  gmouse_new();
  h=0;
  if (h_<3) return;
  if (vsego_<=h_) return;
  x=x_;
  y=y_;
  vsego=vsego_;
  h=h_;
  cchar(x, y, acs.up);
  putcol(bottcol, x, y, 1);
  cchar(x+h-1, y, acs.down);
  putcol(bottcol, x+h-1, y, 1);
  if (scrollcol)
    for (i=x+1; i<x+h-1; i++)
      putcol(scrollcol, i, y, 1);
  barsize=(h*(h-2)-vsego/2)/vsego;
  mouse_ch(x, y, h, 1, _scrollbarc);
  updatescrl(tek_);
  moving=0;
}

void endscroll(void)
{
  if (sptr<0) return;
  sptr--;
  gmouse_on();
  if (sptr<0)
  {
    onkey(_scrollbarc, 0l);
    onkey(_scrollbarupc, 0l);
    onkey(_scrollbardownc, 0l);
    onkey(_scrollbarpgupc,0l);
    onkey(_scrollbarpgdnc, 0l);
  }
  else
  {
    barsize=(h*(h-2)-vsego/2)/vsego;
    updatescrl(tek);
  }
}

int wherescrl(void)
{ return tek;
}

static void scrollbarf(void)
{
  if (moving)
  { scrollmove();
    return;
  }
  if (h==0) return;
  if (mousex==x)
  { /* up */
    /*
    tek--;
    if (tek<0) tek=0;
    */
    if (getshifts() & 3)
      back_key_first(supc);
    else
      back_key_first(upc);
    return;
  }
  else if (mousex==x+h-1)
  { /* down */
    /*
    tek++;
    if (tek+h>vsego)
      tek=vsego-h;
    */
    if (getshifts() & 3)
      back_key_first(sdownc);
    else
      back_key_first(downc);
    return;
  }
  else if (mousex<x+1+upbar)
  { /* page up */
#ifdef SCROLLPAGE
    back_key_first(pgupc);
    return;
#else
    tek-=h-1;
    if (tek<0) tek=0;
#endif
  }
  else if (mousex>x+1+dnbar)
  { /* page down */
#ifdef SCROLLPAGE
    back_key_first(pgdnc);
    return;
#else
    tek+=h-1;
    if (tek+h>vsego)
      tek=vsego-h;
#endif
  }
  else
  { scrollmove();
    return;
  }
#ifndef SCROLLPAGE
  back_key_first(scrollbarc);
  updatescrl(tek);
#endif
}

static void scrollbarpgup(void)
{
  if (h==0) return;
  if ((mousey!=y) || (mousex<=x) || (mousex>x+upbar))
  {
    return;
  }
  if (tek==0) return;
  if (iskey()) return;
#ifdef SCROLLPAGE
  back_key_first(pgupc);
#else
  tek-=h-1;
  if (tek<0) tek=0;
  back_key_first(scrollbarc);
  updatescrl(tek);
#endif
}

static void scrollbarpgdn(void)
{
  if (h==0) return;
  if ((mousey!=y) || (mousex>=x+h-1) || (mousex<x+2+dnbar))
  {
    return;
  }
  if (tek==vsego-h) return;
  if (iskey()) return;
#ifdef SCROLLPAGE
  back_key_first(pgdnc);
#else
  tek+=h-1;
  if (tek+h>vsego)
    tek=vsego-h;
  back_key_first(scrollbarc);
  updatescrl(tek);
#endif
}

static void scrollbarup(void)
{
  if (h==0) return;
  if ((mousey!=y) || (mousex!=x))
  {
    return;
  }
  if (tek==0) return;
  if (iskey()) return;
  /*
  tek--;
  back_key_first(scrollbarc);
  updatescrl(tek);
  */
  if (getshifts() & 3)
    back_key_first(supc);
  else
    back_key_first(upc);
}

static void scrollbardown(void)
{
  if (h==0) return;
  if ((mousey!=y) || (mousex!=x+h-1))
  {
    return;
  }
  if (tek==vsego-h) return;
  if (iskey()) return;
  /*
  tek++;
  back_key_first(scrollbarc);
  updatescrl(tek);
  */
  if (getshifts() & 3)
    back_key_first(sdownc);
  else
    back_key_first(downc);
}

static void scrollmove(void)
{ int i;

  showmouse();
  if (!moving)
  { moving=1;
    gde=mousex-upbar-x-2;
    gde_last=mousex;
  }
  for (;;)
  {
    i=inkey();
    if (i) back_key(i, 0);
    i=askmouse();
    if (i!=1)
    { moving=0;
      hidemouse();
      return;
    }
    if ((mousey!=y) || (mousex<=x) ||
        (mousex>=x+h-1))
      continue;
    if (mousex==gde_last)
      continue;
    if ((mousex<gde_last) && (upbar==0))
      continue;
    if ((mousex>gde_last) && (dnbar==h-3))
      continue;
    gde_last=mousex;
    if (gde_last-gde-x-2<0)
      gde_last=x+2+gde;
    if (gde_last-gde-x-2>h-3-barsize)
      gde_last=h-3-barsize+x+2+gde;
    /* set tek for upbar==gde_last-gde */
    tek=((vsego-h)*(gde_last-gde-x-2)+(h-3-barsize)/2)/(h-3-barsize);
    updatescrl(tek);
    back_key_first(scrollbarc);
    back_key(_scrollbarc,0);
    hidemouse();
    return;
  }
}

void updatescrl(int tek_)
{
  int i;

  if (sptr<0) return;
  if (sptr>=MAXSCRLSTACK) return;
  if (h==0) return;
  tek=tek_;
  if (barsize==h-4)
    upbar=(tek>(vsego-h-1)/2);
  else
    upbar=((h-4-barsize)*(tek-1)+vsego-h-1)/(vsego-h-1);
  for (i=x+1; i<x+1+upbar; i++)
    cchar(i, y, acs.bar3);
  for (; i<=x+1+dnbar; i++)
    cchar(i, y, acs.bar0);
  for (; i<x+h-1; i++)
    cchar(i, y, acs.bar3);
}

#include <stdlib.h>
#include "gulib.h"
#include "keyc.h"

#define MAXMCH       64
#define M_STACK_SIZE (32*1042)

#ifndef BUTTON5_CLICKED
#define BUTTON5_CLICKED 0x100
#endif

#if 0
#define MOUSE_MASK 	(BUTTON1_PRESSED|BUTTON1_RELEASED|BUTTON1_CLICKED|\
            		 BUTTON2_PRESSED|BUTTON2_RELEASED|BUTTON2_CLICKED|\
            		 BUTTON3_PRESSED|BUTTON3_RELEASED|BUTTON3_CLICKED|\
            		 BUTTON4_CLICKED|BUTTON5_CLICKED|\
	    		 BUTTON_SHIFT|BUTTON_CTRL|BUTTON_ALT|\
			 REPORT_MOUSE_POSITION)
#else
#define MOUSE_MASK	ALL_MOUSE_EVENTS
#endif


int  mouse_mode=1;
static int mousexnow, mouseynow;
static int mouse_state;

static struct { int x, y, cx, cy, key; } mch[MAXMCH];
static int levels[16]={0, 0};
static int nlevels=1, showlevels=1;

void gulib_sem(int set);

#ifdef KEY_MOUSE
static int m_key_(MEVENT *mev);
#endif
static int o_reaction_test(int c);

void initmouse(void)
{
  return;
}

void closemouse(void)
{
  return;
}

int askmouse(void)
{
#ifdef KEY_MOUSE
  MEVENT mev;
  int rc;

#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  rc = getmouse(&mev);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
  mousexnow = mev.y;
  mouseynow = mev.x;
  if (rc != ERR) {
	  rc = m_key_(&mev);
	  // ungetmouse(&mev);
  } else {
  	mousex=mousexnow;
  	mousey=mouseynow;
  }
#endif
  return mouse_state;
}

void showmouse(void)
{ 
#ifdef KEY_MOUSE
  //long long l;
  if (mouse==0) return;
  //l=
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  mousemask(MOUSE_MASK, NULL);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
  //printf("Mouse mask: %08Lx (requested %08Lx)\r\n", l, (long long)MOUSE_MASK);
  //printf("REPORT_MOUSE_POSITION is %04lx\r\n", REPORT_MOUSE_POSITION);
#endif
}

void hidemouse(void)
{ 
#ifdef KEY_MOUSE
  if (mouse==0) return;
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  mousemask(0, NULL);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
#endif
}

int m_key(void)
{
#ifdef KEY_MOUSE
  MEVENT mev;
  int rc;

#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  rc = getmouse(&mev);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
  if (rc == ERR) return 0;
  return m_key_(&mev);
#else
  return 0;
#endif
}

#ifdef KEY_MOUSE
static int m_key_(MEVENT *mev)
{
  int key, shifted;

  key = shifted = 0;
  mousexnow = mev->y;
  mouseynow = mev->x;
  //printf("%08Lx\n", (unsigned long long)mev.bstate);
  if (mev->bstate & BUTTON_SHIFT) shifted |= 0x0800;
  if (mev->bstate & BUTTON_CTRL)  shifted |= 0x1000;
  if (mev->bstate & BUTTON_ALT)   shifted |= 0x2000;
  if (mev->bstate & BUTTON1_PRESSED) {
	  mouse_state |= 1;
	  key = mouse_left | shifted;
  }
  if (mev->bstate & BUTTON1_RELEASED)
	  mouse_state &= ~1;
  if (mev->bstate & BUTTON1_CLICKED) {
	  mouse_state &= ~1;
	  key = mouse_left | shifted;
  }
  if (mev->bstate & BUTTON3_PRESSED) {
	  mouse_state |= 2;
	  key = mouse_right | shifted;
  }
  if (mev->bstate & BUTTON3_RELEASED)
	  mouse_state &= ~2;
  if (mev->bstate & BUTTON3_CLICKED) {
	  mouse_state &= ~2;
	  key = mouse_right | shifted;
  }
  if (mev->bstate & BUTTON2_PRESSED) {
	  mouse_state |= 4;
	  key = mouse_middle | shifted;
  }
  if (mev->bstate & BUTTON2_RELEASED)
	  mouse_state &= ~4;
  if (mev->bstate & BUTTON2_CLICKED) {
	  mouse_state &= ~4;
	  key = mouse_middle | shifted;
  }
#if 0
  if (mev.bstate & BUTTON4_CLICKED)
	  key = mouse_upc;
  if (mev.bstate & BUTTON5_CLICKED)
	  key = mouse_downc;
#endif
  if (mouse_mode) {
	  if (key == mouse_right)  key = escc;
	  if (key == mouse_middle) key = f1c;
  }
  if (key) {
	  mousex = mousexnow;
	  mousey = mouseynow;
  }
  return o_reaction_test(key);
}
#endif

static int o_reaction_test(int c)
{ int i;

  if ((c & ~mouse_shift)!=mouse_enter) return c;
  for (i=levels[nlevels-showlevels]; i<levels[nlevels]; i++)
    if ((mch[i].x<=mousex) &&
        (mch[i].y<=mousey) &&
        (mch[i].cx>mousex) &&
        (mch[i].cy>mousey))
      return mch[i].key;
  return mouse_enter;
}

int  mouse_ch(int x, int y, int sx, int sy, int key)
{
  if (levels[nlevels]==MAXMCH)
    return 0;
  mch[levels[nlevels]].x=x;
  mch[levels[nlevels]].y=y;
  mch[levels[nlevels]].cx=x+sx;
  mch[levels[nlevels]].cy=y+sy;
  mch[levels[nlevels]].key=key;
  levels[nlevels]++;
  return 0;
}

void gmouse_cancel(void)
{ showlevels=nlevels=1;
  levels[1]=0;
}

int  gmouse_off(void)
{
  if (nlevels<sizeof(levels)/sizeof(levels[0]))
  { nlevels++;
    showlevels=1;
    levels[nlevels]=levels[nlevels-1];
  }
  return 0;
}

int  gmouse_on(void)
{
  if (nlevels>1)
  { nlevels--;
    if (showlevels>1)
      showlevels--;
  }
  return 0;
}

int  gmouse_addold(void)
{
  if (showlevels>1)
    showlevels++;
  return 0;
}

int  gmouse_new(void)
{
  if (nlevels<sizeof(levels)/sizeof(levels[0]))
  { nlevels++;
    showlevels++;
    levels[nlevels]=levels[nlevels-1];
  }
  return 0;
}

int  gmouse_offold(void)
{
  if (showlevels>1)
    showlevels--;
  return 0;
}

void gmouse_offallold(void)
{ showlevels=1;
}

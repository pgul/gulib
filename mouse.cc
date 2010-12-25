#include <stdlib.h>
#include "gulib.h"
#include "keyc.h"

#define MAXMCH       64
#define M_STACK_SIZE (32*1042)

HMOU hmouse=-1;
char mouse_rep_param=0;
int  mouse_mode=1;
static int gr_mode;
static int mouse_stack=1;
static int  delay1=250, delay2=33;
static rep_stack[M_STACK_SIZE], m_stack[M_STACK_SIZE];
static TID reptid=-1, m_tid=-1;
static int old_state=0;
static HMTX hsem=-1;
static int mousexnow, mouseynow;

static struct { int x, y, cx, cy, key; } mch[MAXMCH];
static int levels[16]={0, 0};
static int nlevels=1, showlevels=1;

int o_reaction_test(int c);

#if 0
static void repmouse(void *par)
{ int c = (int)par;

  DosSleep(delay1);
  for (;;)
  { askmouse();
    back_key(o_reaction_test(c), 0);
    DosSleep(delay2);
  }
}

void mouse_stop_rep(void)
{ 
  if (reptid==-1) return;
  DosKillThread(reptid);
  DosWaitThread(&reptid, DCWW_WAIT);
  reptid=-1;
}

void _domouserep(int c)
{
  if (reptid!=-1) return;
  reptid=_beginthread(repmouse, rep_stack, M_STACK_SIZE, (char *)(c));
  DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, reptid);
}
#endif

static void m_key(void *par)
{
  MOUEVENTINFO info;
  int pressed, released, c, sh;
  USHORT len=2, mode=1; /* with wait */
  char cell[4];
  
  DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
  for (;;)
  { 
    if (MouReadEventQue(&info, &mode, hmouse))
    { DosSleep(30);
      continue;
    }
    if ((mousexnow!=info.row) || (mouseynow!=info.col))
    { DosRequestMutexSem(hsem, 10000);
      if ((mouse_stack==0) && (!gr_mode))
      { VioReadCellStr(cell, &len, mousexnow, mouseynow, 0);
        cell[1]^=0x7f;
        VioWrtNAttr((PBYTE)&cell[1], 1, mousexnow, mouseynow, 0);
        VioReadCellStr(cell, &len, info.row, info.col, 0);
        cell[1]^=0x7f;
        VioWrtNAttr((PBYTE)&cell[1], 1, info.row, info.col, 0);
      }
      mousexnow=info.row;
      mouseynow=info.col;
      DosReleaseMutexSem(hsem);
    } 
    info.fs|=info.fs>>1;
    info.fs&=0x2a; /* buttons only */
    if (old_state==info.fs) continue;
    pressed=info.fs & (info.fs ^ old_state);
    released=(~info.fs) & (info.fs ^ old_state);
    old_state=info.fs;
  /*
  printf("Pressed %x, released %x, new state %x\n", pressed, released,old_state);
  */
    mouse_stop_rep();
    if (!pressed) continue;
    mousexg=mousexnow*8;
    mouseyg=mouseynow*8;
    sh=getshifts();
    if (sh==-1) sh=0;
    c=0xc0;
    if (sh & 3) /* shift */
      c|=0x8;
    if (sh & 4) /* control */
      c|=0x10;
    if (sh & 8) /* alt */
      c|=0x20;
    if (info.fs &    2) c|=1;
    if (info.fs &    8) c|=2;
    if (info.fs & 0x20) c|=4;
    c<<=8;
    if (mouse_mode)
      switch (c)
      { case mouse_right:   c = escc;
                            break;
        case mouse_middle:  c = f1c;
                            break;
      }
    back_key(o_reaction_test(c), 0);
    if (mouse_rep_param)
      _domouserep(c);
  }
}

void initmouse(void)
{ static USHORT mask=0x54, status=0x100;
  int  c;
  PTIB ptib;
  PPIB ppib;


  if (mouse==0) return;
  /*
  printf("old_state %x\n",old_state);
  */
  c=MouOpen(NULL, &hmouse);
  if (c)
  { mouse=0;
    hmouse=-1;
    return;
  }
  atexit(mclose);
  DosGetInfoBlocks(&ptib, &ppib);
  if (ppib->pib_ultype!=0)
    gr_mode=1;
  else
    gr_mode=0;
  MouSetDevStatus(&status, hmouse);
  if (!gr_mode) mask=0x7f;
  MouSetEventMask(&mask, hmouse);
  DosCreateMutexSem(NULL, &hsem, 0, 0);
  m_tid=_beginthread(m_key, m_stack, M_STACK_SIZE, "");
  /*
  printf("old_state %x\n",old_state);
  */
}

int askmouse(void)
{
  PTRLOC loc;
  int state;
  
  if (hmouse==(HMOU)-1) return 0;
  MouGetPtrPos(&loc, hmouse);
  mousexg=loc.row*8;
  mouseyg=loc.col*8;
  state=0;
  if (old_state &    2) state|=1;
  if (old_state &    8) state|=2;
  if (old_state & 0x20) state|=4;
  return state;
}

void showmouse(void)
{ 
  char c[4];
  USHORT len=2;

  if (hmouse==(HMOU)-1) return;
  if (mouse_stack==0)
    return; /* already visible */
  if (--mouse_stack>0)
    return; /* still hide */
  if (!gr_mode)
  { DosRequestMutexSem(hsem, 10000);
    VioReadCellStr((PBYTE)&c, &len, mousexnow, mouseynow, 0);
    c[1]^=0x7f;
    VioWrtNAttr(&c[1], 1, mousexnow, mouseynow, 0);
    DosReleaseMutexSem(hsem);
  }
}

void hidemouse(void)
{ 
  char c[4];
  USHORT len=2;
  
  if (hmouse==(HMOU)-1) return;
  if (mouse_stack++>0)
    return; /* already hide */
  if (!gr_mode)
  { DosRequestMutexSem(hsem,10000);
    askmouse();
    VioReadCellStr((PBYTE)&c,&len,mousexnow,mouseynow,0);
    c[1]^=0x7f;
    VioWrtNAttr(&c[1],1,mousexnow,mouseynow,0);
    DosReleaseMutexSem(hsem);
  }
}

int o_reaction_test(int c)
{ int i;

  if ((c & ~mouse_shift)!=mouse_enter) return c;
  for (i=levels[nlevels-showlevels];i<levels[nlevels];i++)
    if ((mch[i].x<=gmousex) &&
        (mch[i].y<=gmousey) &&
        (mch[i].cx>gmousex) &&
        (mch[i].cy>gmousey))
      return mch[i].key;
  return mouse_enter;
}

int  mouse_ch(int x,int y,int sx,int sy,int key)
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

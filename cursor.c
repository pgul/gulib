#include <string.h>
#include "gulib.h"
#include "keyc.h"

static int  crsr_pstack=-1;
static struct { int x, y; } crsr_stack[32];

void gulib_sem(int set);
void place_nolock(int x, int y);

int  crsr_save(void)
{
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  crsr_pstack++;
  crsr_stack[crsr_pstack].x = curx();
  crsr_stack[crsr_pstack].y = cury();
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
  return 0;
}

int  crsr_restore(void)
{ 
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  if (crsr_pstack>=00)
  { place_nolock(crsr_stack[crsr_pstack].x, crsr_stack[crsr_pstack].y);
    crsr_pstack--;
  }
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
  return 0;
}

void crsr_hide(void)
{
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  curs_set(0);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
}

void crsr_show(void)
{
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(1);
#endif
  curs_set(1);
#ifdef WITH_PTHREAD_MUTEX
  gulib_sem(0);
#endif
}

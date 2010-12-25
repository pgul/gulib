#include <stdarg.h>
#include <errno.h>
#include "gulib.h"
#include "keyc.h"

#define MAXONKEY 100
#define Q_KEY_SIZE 64

int (*inkey)(void)=_inkey;

static int at_onkey=0;
static int onkeys=0;
static struct {
                int key;
                void (*func)(void);
              } onkeyarr[MAXONKEY];
static int q_key_beg=0, q_key_end=0;
static int q_key[Q_KEY_SIZE];
static int q_key_mousex[Q_KEY_SIZE], q_key_mousey[Q_KEY_SIZE];
static int shift_state = 0x80;

int getshifts(void)
{
  return shift_state;
}

static int conv_key(int key)
{
    switch (key) {
	case KEY_DOWN:		return downc;
	case KEY_UP:		return upc;
	case KEY_LEFT:		return leftc;
	case KEY_RIGHT:		return rightc;
	case KEY_HOME:		return homec;
	case KEY_BACKSPACE:	return bkspcc;
	case KEY_F(1):		return f1c;
	case KEY_F(2):		return f2c;
	case KEY_F(3):		return f3c;
	case KEY_F(4):		return f4c;
	case KEY_F(5):		return f5c;
	case KEY_F(6):		return f6c;
	case KEY_F(7):		return f7c;
	case KEY_F(8):		return f8c;
	case KEY_F(9):		return f9c;
	case KEY_F(10):		return f10c;
	case KEY_F(11):		return f11c;
	case KEY_F(12):		return f12c;
#ifdef KEY_DC
	case KEY_DC:		return del2c;
#endif
#ifdef KEY_IC
#if 0
	case KEY_IC:		shift_state |= 0x80;  return delc;
#else
	case KEY_IC:		return insc;
#endif
#endif
#ifdef KEY_EIC
	case KEY_EIC:		shift_state &= ~0x80; return delc;
#endif
	case KEY_NPAGE:		return pgdnc;
	case KEY_PPAGE:		return pgupc;
	case KEY_END:		return endc;
#ifdef KEY_SLEFT
	case KEY_SLEFT:		return sleftc;
#endif
#ifdef KEY_SRIGHT
	case KEY_SRIGHT:	return srightc;
#endif
#ifdef KEY_BTAB
	case KEY_BTAB:		return stabc;
#endif
	case '\t':		return tabc;
	case '\r':		return enterc;
	case '\n':		return centerc;
	case ' ':		return spacec;
	case '\x1b':		return escc;
	case 0x8500:		return sf1c;
	case 0x8600:		return sf2c;
	case 0x115:		return sf1c;
	case 0x116:		return sf2c;
	case 0x117:		return sf3c;
	case 0x118:		return sf4c;
	case 0x119:		return sf5c;
	case 0x11a:		return sf6c;
	case 0x11b:		return sf7c;
	case 0x11c:		return sf8c;
	case 0x11d:		return sf9c;
	case 0x11e:		return sf10c;
	case 0x11f:		return sf11c;
	case 0x120:		return sf12c;
	case 0x121:		return cf1c;
	case 0x122:		return cf2c;
	case 0x123:		return cf3c;
	case 0x124:		return cf4c;
	case 0x125:		return cf5c;
	case 0x126:		return cf6c;
	case 0x127:		return cf7c;
	case 0x128:		return cf8c;
	case 0x129:		return cf9c;
	case 0x12a:		return cf10c;
	case 0x12b:		return cf11c;
	case 0x12c:		return cf12c;
	case 0x139:		return altf1c;
	case 0x13a:		return altf2c;
	case 0x13b:		return altf3c;
	case 0x13c:		return altf4c;
	case 0x13d:		return altf5c;
	case 0x13e:		return altf6c;
	case 0x13f:		return altf7c;
	case 0x140:		return altf8c;
	case 0x141:		return altf9c;
	case 0x142:		return altf10c;
	case 0x143:		return altf11c;
	case 0x144:		return altf12c;
    }
    return key;
}

static int kbhit(void)
{
  int i;

  nodelay(stdscr, TRUE);
  i = getch();
  nodelay(stdscr, FALSE);
  //printf("kbhit: getch returns 0x%04x\r\n", i);
  if (i == ERR) return (errno == EINTR ? 0 : ERR);
#ifdef KEY_MOUSE
  if (i == KEY_MOUSE) i=m_key();
#endif
  i = conv_key(i);
  if (i != 0) back_key_first(i);
  return i;
}

int _inkeym(int do_refresh)
{
  int rc, i;

  if (q_key_beg!=q_key_end)
  {
    mousex=q_key_mousex[q_key_beg];
    mousey=q_key_mousey[q_key_beg];
    rc=q_key[q_key_beg++];
    q_key_beg%=Q_KEY_SIZE;
    goto chk_onkey;
  }
  if (do_refresh) g_refresh();
  showmouse();
  rc=getch();
  hidemouse();
  if (rc == ERR) return (errno == EINTR ? 0 : ERR);
#ifdef KEY_MOUSE
  if (rc == KEY_MOUSE)
    rc = m_key();
#endif
  rc = conv_key(rc);
chk_onkey:
  if (rc && !at_onkey)
  { for (i=0; i<onkeys; i++)
      if (onkeyarr[i].key==rc)
      { at_onkey=1;
        onkeyarr[i].func();
        at_onkey=0;
        return 0;
      }
  }
  return rc;
}

int _inkey(void)
{
  int rc;

  while ((rc=_inkeym(1))==0);
  return rc;
}

int _inkey_norefresh(void)
{
  int rc;

  while ((rc=_inkeym(0))==0);
  return rc;
}

int iskey(void)
{ if (q_key_beg!=q_key_end)
    return q_key[q_key_beg];
  return kbhit();
}

void onkey(int key, void (*reaction)(void))
{ int i;

  for (i=0; i<onkeys; i++)
    if (onkeyarr[i].key==key)
      break;
  if (i==onkeys)
  { if (onkeys==MAXONKEY) return;
    if (reaction==NULL) return;
  }
  if (reaction==NULL)
  { for(i++; i<onkeys; i++)
    { onkeyarr[i-1].key=onkeyarr[i].key;
      onkeyarr[i-1].func=onkeyarr[i].func;
    }
    onkeys--;
    return;
  }
  onkeyarr[onkeys].key=key;
  onkeyarr[onkeys++].func=reaction;
}

void back_key_first(int c)
{
  if ((q_key_end+1)%Q_KEY_SIZE==q_key_beg)
    return;
  q_key_beg=(q_key_beg+Q_KEY_SIZE-1)%Q_KEY_SIZE;
  q_key_mousex[q_key_beg]=mousex;
  q_key_mousey[q_key_beg]=mousey;
  q_key[q_key_beg]=c;
}

static void back_one_key(int c)
{
  if ((q_key_end+1)%Q_KEY_SIZE==q_key_beg)
    return;
  q_key_mousex[q_key_end]=mousex;
  q_key_mousey[q_key_end]=mousey;
  q_key[q_key_end++]=c;
  q_key_end%=Q_KEY_SIZE;
}

int back_key(int key, ...)
{
  va_list keys;
  int c;
  
  if (key==0) return 0;
  back_one_key(key);
  va_start(keys, key);
  for (;;)
  { c = va_arg(keys, int);
    if (c==0)
    { va_end(keys);
      return 0;
    }
    back_one_key(c);
  }
}

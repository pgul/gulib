#include <stdlib.h>
#include "gulib.h"
#include "keyc.h"

int smenu(chtype cv1, chtype cv2,
		   int x, int y, int sx, int sy,
		   int *pole, int *gde, char *helpline,
		   char *(*proc)(int), int max)
{
  int up, cur, i;

  if (sx>max) sx=max;
  ramka(cv1, x, y, sx+2, sy+2);   /* evakuate + border */
  cnline(x+sx+1, y+1, helpline, sy);
  if (max==0)
  { crsr_save();
    crsr_hide();
    i=inkey();
    crsr_restore();
    return i;
  }
  crsr_save();
  crsr_hide();   /* turn off cursor */
  up = *pole-*gde;
  cur=*gde;
newup:
  if (up+sx>max) up=max-sx;
  if (up<0) up=0;
  for (i=0; i<sx; i++)
  { if (i+up==max) break;
    fill(' ', x+i+1, y+1, sy); /* fill_ch */
    cnline(x+i+1, y+1, proc(i+up), sy);   /* parameters order */
  }
newcur:
  if (cur<0) cur=0;
  if (cur>=sx) cur=sx-1;
  putcol(cv2, x+cur+1, y, sy+2);  /* ch_attr */
#define refill() putcol(cv1, x+cur+1, y, sy+2);
badk:
  switch (i=inkey()) /* getkey */
  { case upc:
	  if (cur)
	  { refill();
	    cur--;
	    goto newcur;
	  }
	  if (up)
	  { refill();
	    up--;
	    goto newup;
	  }
	  goto badk;
    case downc:
	  if (cur<sx-1)
	  { refill();
	    cur++;
	    goto newcur;
	  }
	  if (cur+up < max-1)
	  { refill();
	    up++;
	    goto newup;
	  }
	  goto badk;
  case pgupc: m_upc:
	  if (cur)
	  { refill();
	    cur=0;
	    goto newcur;
	  }
	  if (up)
	  { refill();
	    up-=sx;
	    goto newup;
	  }
	  goto badk;
  case pgdnc: m_downc:
	  if (up+sx==max)
	  { refill();
	    cur=sx-1;
	    goto newcur;
	  }
	  refill();
	  up+=sx;
	  cur=0;
	  goto newup;
  case homec:
	  refill();
	  cur=up=0;
	  goto newup;
  case endc:
	  refill();
	  up=max-sx;
	  if (up<0) up=0;
	  cur=max-1 - up;
	  goto newup;
  case mouse_enter:
	  if (mousex==x) goto m_upc;
	  if (mousex==x+sx+1) goto m_downc;
	  if ((mousex>x) && (mousex<x+sx+1))
	  { i=mousex-x-1;
	    if (i==cur)
	    { i=enterc;
	      goto m_enterc;
	    }
	    refill();
	    cur=i;
	    goto newcur;
	  }
	  goto badk;
  default: m_enterc:
	  *pole=up+cur;
	  *gde=cur;
	  crsr_restore();
	  return i;
  }
}

/*
char str[100];
char * p(int x)
{
return itoa(x,str,10);
}

void main(void)
{
int pole=0;
initmouse();
smenu(bluef+white, bluef+red+very,
		2,20,
		15,30,
		&pole,p,100);
}
*/

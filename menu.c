#include <stdarg.h>
#include "gulib.h"
#include "keyc.h"

int menu( chtype cv1,chtype cv2,
	  int x, int y,
	  int sx,int sy,
	  int *pole,...)
{ int i;
  va_list stroki;

  va_start(stroki, pole);
  ramka(cv1, x, y, sx+2, sy+2);
  for (i=0; i<sx; i++)
     cline(x+i+1, y+1, va_arg(stroki, char*));
  va_end(stroki);
  if (!pole) return(0);
  if (sx==0) { inkey();
	       return 0;
	     }
  i= *pole+x;
  crsr_save();
  crsr_hide();
  for (;;)
  { putcol(cv2, i, y, sy+2);
    switch(inkey())
    { case downc: putcol(cv1, i, y, sy+2);
	          if (i!=x+sx) ++i;
		  break;
      case upc:   putcol(cv1, i, y, sy+2);
	          if(i!=x+1) --i;
		  break;
      case homec:
      case pgupc: putcol(cv1, i, y, sy+2);
	          i=x+1;
		  break;
      case endc:
      case pgdnc: putcol(cv1, i, y, sy+2);
	          i=x+sx;
		  break;
      case mouse_enter:
		  if (mousex != i)
	          { if (mousex>x && mousex<=x+sx)
		    { putcol(cv1, i, y, sy+2);
		      i=mousex;
		    }
		    break;
	          }
      case enterc:*pole=i-x;
		  crsr_restore();
		  return(1);
      case escc:  *pole=i-x;
		  crsr_restore();
		  return(0);
    }
  }
}

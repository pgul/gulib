#include <stdarg.h>
#include <string.h>
#include "gulib.h"
#include "keyc.h"

extern char edit_updown;
extern int editptr;
int mkorr(chtype cv1, chtype cv2,
	  int x,  int y,
	  int sx, int sy, int psize,
/*	  char *prompt, char *line, int tip, */
	  ...)
{ int mt[25];
  char *ms[25];
  int msptr=0;
  va_list arg;
  int i, j, c;

  ramka(cv1, x, y, sx, sy);
  ++x;
  ++y;
  sx-=2;
  sy-=2;
  va_start(arg, psize);
  for (i=0; i<sx; i++)
  {
    cline(x+i, y, va_arg(arg, char*));
    ms[msptr]=va_arg(arg, char*);
    j=mt[msptr]=va_arg(arg, int);
    putcol(cv2, x+i, y+psize, j>0 ? strlen(ms[msptr])+1 : strlen(ms[msptr]));
    if (j>0)
    { cnline(x+i, y+psize, ms[msptr], j);
      cnline(x+i, y+psize+j, ".", 1);
      cline(x+i, y+psize+j+1, ms[msptr]+j);
    } else
      cline(x+i, y+psize, ms[msptr]);
    msptr++;
  }
  va_end(arg);
  i=0;
  edit_updown=1;
  editptr=0;
  for(;;)
    switch(c=edit(x+i, y+psize, ms[i], mt[i], 0))
    {
      case escc:   edit_updown=0;
	           return(0);
      case upc:    if (i!=0) i--;
	           continue;
      case enterc: if (i<sx-1)
		   { i++;
		     editptr=0;
		     continue;
		   }
      case f1c :
      case f2c :
      case f3c :
      case f4c :
      case f5c :
      case f6c :
      case f7c :
      case f8c :
      case f9c :
      case pgupc:
      case pgdnc:
      case f10c:   edit_updown=0;
	           return(c);
      case downc:  if (i<sx-1) i++;
	           continue;
      default:     continue;
   }
}


#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "gulib.h"
#include "keyc.h"

#define MAXPOLEY 10

int quere(chtype col1, chtype col2, int x, int y, int strmes, int poley,
          int *pole, ...)
{
  char begpole[MAXPOLEY], lenpole[MAXPOLEY];
  char *hotkeys[MAXPOLEY];
  char *line;
  int  i, l;
  int  sy, spc, spc1;
  va_list arg;

  va_start(arg, pole);

  /* calculate width of the ramka */
  sy=0;
  for (i=0; i<strmes; i++)
  { l=strlen(va_arg(arg, char *));
    if (l>sy) sy=l;
  }
  l=0;
  for (i=0; i<poley; i++)
  {
    l+=strlen(va_arg(arg, char *));
    hotkeys[i]=va_arg(arg, char *);
  }
  if (l>sy) sy=l;
  if (poley>MAXPOLEY)
  { va_end(arg);
    return -1;
  }
  if (pole)
    if (((*pole<0) || (*pole>=poley)) && poley)
      *pole=0;
  crsr_save();
  crsr_hide();
  /* draw */
  ramka(col1, x, y, strmes+2+(poley!=0), sy+2);
  va_end(arg);
  va_start(arg, pole);
  for (i=0; i<strmes; i++)
  {
    line=va_arg(arg, char *);
    cline(x+i+1, y+1+(sy-strlen(line))/2, line);
  }
  l=sy-l;
  spc=l/(poley+1);
  spc1=l-spc*(poley+1);
  x+=strmes+1;
  for (i=0; i<poley; i++)
  {
    if (!i)
      begpole[i]=y+1+spc;
    else
      begpole[i]=begpole[i-1]+lenpole[i-1]+spc;
    if (spc1!=0)
    { begpole[i]++;
      spc1--;
    }
    line=va_arg(arg, char *);
    cline(x, begpole[i], line);
    lenpole[i]=strlen(line);
    line=va_arg(arg, char *);
  }
  va_end(arg);
  if (!poley)
  { i=inkey();
    remenu();
    crsr_restore();
    return i;
  }
  putcol(col2, x, begpole[*pole], lenpole[*pole]);
  /* run over the menu */
  for (;;)
  {
    switch (i=inkey())
    {
      case enterc:  
      /* case escc: */
                    remenu();
                    crsr_restore();
                    return i;
      case leftc:   if (pole==NULL) continue;
                    putcol(col1, x, begpole[*pole], lenpole[*pole]);
                    (*pole)--;
                    if (*pole<0)
                      *pole=poley-1;
                    putcol(col2, x, begpole[*pole], lenpole[*pole]);
                    continue;
      case rightc:  if (pole==NULL) continue;
                    putcol(col1, x, begpole[*pole], lenpole[*pole]);
                    (*pole)++;
                    if (*pole>=poley)
                      *pole=0;
                    putcol(col2, x, begpole[*pole], lenpole[*pole]);
                    continue;
      case mouse_enter:
                    if (mousex!=x) continue;
                    for (i=0; i<poley; i++)
                      if ((mousey>=begpole[i]) && (mousey<begpole[i]+lenpole[i]))
		      {
                        if (i==*pole)
                        { remenu();
                          crsr_restore();
                          return mouse_enter;
                        }
                        else
                        { putcol(col1, x, begpole[*pole], lenpole[*pole]);
                          *pole=i;
                          putcol(col2, x, begpole[*pole], lenpole[*pole]);
                          break;
                        }
		      }
                    continue;
      default:      if (!(char)i) continue;
                    for (l=0; l<poley; l++)
                      if (strchr(hotkeys[l], (char)i))
                      {
                        remenu();
                        crsr_restore();
                        if (pole)
                          *pole=l;
                        return i;
                      }
                    continue;
    }
  }
}

#include <string.h>
#include "gulib.h"
#include "keyc.h"

#define SPACESTR "                                                                                 "

static char ** _lines;
int scrlmenu_redraw=1;
extern int updated_scr;

static char *line(int num, chtype *col)
{ return _lines[num];
}

void scrollup(int x, int y, int sx, int sy)
{ int i, maxx, maxy;

  graph(&maxx, &maxy, NULL);
  for (i=0; i<sx-1; i++)
    memcpy(saved_scr+(x+i)*maxy+y, saved_scr+(x+i+1)*maxy+y, sy*sizeof(chpair));
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

void scrolldown(int x, int y, int sx, int sy)
{ int i, maxx, maxy;

  graph(&maxx, &maxy, NULL);
  for (i=sx-1; i>0; i--)
    memcpy(saved_scr+(x+i)*maxy+y, saved_scr+(x+i-1)*maxy+y, sy*sizeof(chpair));
  updated_scr=1;
#ifdef DEBUG
  g_refresh();
#endif
}

int scrlmenu(chtype col1, chtype col2, int x, int y, int sx, int sy,
             char *head, int *pole, int *upline, int poley,
             char *lines[])
{
  _lines=lines;
  return scrlmenuf(col1, col2, x, y, sx, sy, head, pole, upline, poley, line);
}

int scrlmenuf(chtype col1, chtype col2, int x, int y, int sx, int sy,
              char *head, int *pole, int *upline, int poley,
              char *(*line)(int num, chtype *col))
{
  int  i, j;
  chtype linecol;
  char *p;

  crsr_save();
  crsr_hide();
  /* 1. Correctness of upline & pole */
  if (*upline<0) *upline=0;
  if (*pole>=poley) *pole=poley-1;
  if (*pole<0) *pole=0;
  if ((*upline>poley-sx) && (*upline))
  { *upline=poley-sx;
    if (*upline<0) *upline=0;
  }
  if (*upline>*pole) *upline=*pole;
  if (*upline+sx<=*pole)
    *upline=*pole-sx+1;
  /* 2. Draw ramka */
  if (scrlmenu_redraw)
  { if (mramka(col1, x, y, sx, sy)) return -1;
  }
  else
  { putcol(col2, x+*pole-*upline+1, y+1, sy);
    barputcol(col1, x+1, y+1, *pole-*upline, sy);
    barputcol(col1, x+*pole-*upline+2, y+1, sx-(*pole-*upline+1), sy);
  }
  i=sy-strlen(head);
  if (i<0) i=0;
  if (scrlmenu_redraw)
    cnline(x, y+1+(i>>1), head, sy);
  /* 3. Choose */
  /* Draw scroll-bar */
  drawscroll(x+1, y+sy+1, sx, poley, *upline);
  for (;;)
  {
    for (i=0; (i<sx) && (i<poley); i++)
    {
      linecol=col1;
      p=line(i+*upline, &linecol);
      cnline(x+i+1,y+1,p,sy);
      putcol(linecol, x+i+1, y+1, sy);
      if (strlen(p)<sy)
        cnline(x+i+1,y+1+strlen(p),SPACESTR,sy-strlen(p));
    }
    putcol(col2, x+*pole-*upline+1, y+1, sy);
    for (; i<sx; i++)
      cnline(x+i+1, y+1, SPACESTR, sy);
    updatescrl(*upline);
    for (;;)
    { putcol(col2, x+*pole-*upline+1, y+1, sy);
donothing:
      i=inkey();
      if (getshifts() & SCROLLLOCK_ON)
        if ((i==upc) || (i==downc) || (i==supc) || (i==sdownc))
          i^=0x10000;
      switch (i)
      {
        case upc:    if (*pole>*upline)
                     { linecol=col1;
                       line(*pole, &linecol);
                       putcol(linecol, x+*pole-*upline+1, y+1, sy);
                       --*pole;
                       continue;
                     }
                     else if (*upline)
                     { --*upline;
                       --*pole;
                       scrolldown(x+2, y+1, sx-1, sy);
                       linecol = col1;
                       p=line(*pole, &linecol);
                       cnline(x+*pole-*upline+1, y+1, p, sy);
                       if (strlen(p)<sy)
                         cnline(x+*pole-*upline+1, y+1+strlen(p), SPACESTR, sy-strlen(p));
                       linecol = col1;
                       p=line(*pole+1, &linecol);
                       cnline(x+*pole-*upline+2, y+1, p, sy);
                       if (strlen(p)<sy)
                         cnline(x+*pole-*upline+2, y+1+strlen(p), SPACESTR, sy-strlen(p));
                       putcol(linecol, x+*pole-*upline+2, y+1, sy);
                       updatescrl(*upline);
                       continue;
                     }
                     goto donothing;
        case downc:  
                     if ((*pole<*upline+sx-1) && (*pole<poley-1))
                     { linecol=col1;
                       line(*pole, &linecol);
                       putcol(linecol, x+*pole-*upline+1, y+1, sy);
                       ++*pole;
                       continue;
                     }
                     if (*pole<poley-1)
                     { ++*upline;
                       ++*pole;
                       scrollup(x+1, y+1, sx-1, sy);
                       linecol=col1;
                       p=line(*pole, &linecol);
                       cnline(x+*pole-*upline+1, y+1, p, sy);
                       if (strlen(p)<sy)
                         cnline(x+*pole-*upline+1, y+1+strlen(p), SPACESTR, sy-strlen(p));
                       linecol=col1;
                       p=line(*pole-1, &linecol);
                       cnline(x+*pole-*upline, y+1, p, sy);
                       if (strlen(p)<sy)
                         cnline(x+*pole-*upline, y+1+strlen(p), SPACESTR, sy-strlen(p));
                       putcol(linecol, x+*pole-*upline, y+1, sy);
                       updatescrl(*upline);
                       continue;
                     }
                     goto donothing;
        case pgupc:  if (*pole==0) goto donothing;
                     linecol = col1;
                     line(*pole, &linecol);
                     putcol(linecol, x+*pole-*upline+1, y+1, sy);
                     if (*pole!=*upline)
                     { *pole=*upline;
                       continue;
                     }
                     *pole-=sx;
                     *upline-=sx;
                     if (*upline<0) *upline=0;
                     if (*pole<0) *pole=0;
                     break;
        case pgdnc:  if (*pole>=poley-1) goto donothing;
                     linecol = col1;
                     line(*pole, &linecol);
                     putcol(linecol, x+*pole-*upline+1, y+1, sy);
                     if (*pole!=*upline+sx-1)
                     { *pole=*upline+sx-1;
                       if (*pole>=poley) *pole=poley-1;
                       continue;
                     }
                     *pole+=sx;
                     *upline+=sx;
                     if (*pole>=poley) *pole=poley-1;
                     if (*upline+sx>poley)
                     { *upline=poley-sx;
                       if (*upline<0) *upline=0;
                     }
                     break;
#if 0
        case homec:  if (*pole==*upline) goto donothing;
                     linecol = col1;
                     line(*pole, &linecol);
                     putcol(linecol, x+*pole-*upline+1, y+1, sy);
                     *pole=*upline;
                     continue;
        case endc:   if (*pole==*upline+sx-1) goto donothing;
                     linecol = col1;
                     line(*pole, &linecol);
                     putcol(linecol, x+*pole-*upline+1, y+1, sy);
                     *pole=*upline+sx-1;
                     if (*pole>=poley) *pole=poley-1;
                     continue;
#endif
	case homec:
        case cpgupc:
        case chomec: if (*pole==0) goto donothing;
                     linecol = col1;
                     line(*pole, &linecol);
                     putcol(linecol, x+*pole-*upline+1, y+1, sy);
                     *upline=*pole=0;
                     break;
	case endc:
        case cpgdnc:
        case cendc:  if (*pole>=poley-1) goto donothing;
                     linecol = col1;
                     line(*pole, &linecol);
                     putcol(linecol, x+*pole-*upline+1, y+1, sy);
                     *upline=poley-sx;
                     if (*upline<0) *upline=0;
                     *pole=poley-1;
                     break;
        case supc:   
                     if (*upline==0) goto donothing;
                     /*
                     putcol(col1,x+*pole-*upline+1,y+1,sy);
                     --*upline;
                     --*pole;
                     i=(col1<<8) + ' ';
                     VioScrollDn(x+1,y+1,x+sx,y+sy,1,&i,0);
                     p=line(*upline);
                     cnline(x+1,y+1,p,sy);
                     if (strlen(p)<sy)
                       cnline(x+1,y+1+strlen(p),SPACESTR,sy-strlen(p));
                     updatescrl(*upline);
                     */
                     --*upline;
                     --*pole;
                     if (*pole-*upline>1)
                       scrolldown(x+1, y+1, *pole-*upline-2, sy);
                     linecol = col1;
                     p = line(*upline, &linecol);
                     if (*pole-*upline>0)
                       putcol(linecol,x+1,y+1,sy);
                     cnline(x+1, y+1, p, sy);
                     if (strlen(p)<sy)
                       cnline(x+1, y+1+strlen(p), SPACESTR, sy-strlen(p));
                     linecol=col1;
                     p=line(*pole, &linecol);
                     cnline(x+*pole-*upline+1, y+1, p, sy);
                     if (strlen(p)<sy)
                       cnline(x+*pole-*upline+1 ,y+1+strlen(p), SPACESTR, sy-strlen(p));
                     if (sx+*upline-*pole>2)
                       scrolldown(x+*pole-*upline+2, y+1, sx+*upline-*pole-3, sy);
                     if (*pole-*upline<sx-1)
                     { linecol=col1;
                       p=line(*pole+1, &linecol);
                       cnline(x+*pole-*upline+2, y+1, p, sy);
                       if (strlen(p)<sy) cnline(x+*pole-*upline+2, y+1+strlen(p), SPACESTR, sy-strlen(p));
                     }
                     updatescrl(*upline);
                     goto donothing;
        case sdownc:
                     if (*upline+sx>=poley) goto donothing;
                     /*
                     putcol(col1,x+*pole-*upline+1,y+1,sy);
                     ++*upline;
                     ++*pole;
                     i=(col1<<8) + ' ';
                     VioScrollUp(x+1,y+1,x+sx,y+sy,1,&i,0);
                     p=line(*upline+sx-1);
                     cnline(x+sx,y+1,p,sy);
                     if (strlen(p)<sy)
                       cnline(x+sx,y+1+strlen(p),SPACESTR,sy-strlen(p));
                     updatescrl(*upline);
                     */
                     ++*upline;
                     ++*pole;
                     if (*pole-*upline>1)
                       scrollup(x+1, y+1, *pole-*upline-2, sy);
                     if (*pole>*upline)
                     { linecol=col1;
                       p=line(*pole-1, &linecol);
                       cnline(x+*pole-*upline, y+1, p, sy);
                       if (strlen(p)<sy) cnline(x+*pole-*upline, y+1+strlen(p), SPACESTR, sy-strlen(p));
                     }
                     linecol=col1;
                     p=line(*pole, &linecol);
                     cnline(x+*pole-*upline+1, y+1, p, sy);
                     if (strlen(p)<sy) cnline(x+*pole-*upline+1, y+1+strlen(p), SPACESTR, sy-strlen(p));
                     if (sx+*upline-*pole>2)
                       scrollup(x+*pole-*upline+2, y+1, sx+*upline-*pole-3, sy);
                     if (*pole-*upline<sx-1)
                     { linecol=col1;
                       p=line(*upline+sx-1, &linecol);
                       cnline(x+sx, y+1, p, sy);
                       if (strlen(p)<sy)
                         cnline(x+sx, y+1+strlen(p), SPACESTR, sy-strlen(p));
                     }
                     updatescrl(*upline);
                     goto donothing;
#if 1
        case scrollbarc:
                     i=*upline;
                     j=*pole;
                     *upline=wherescrl();
                     if (*upline==i) goto donothing;
                     if (*pole<*upline)
                       *pole=*upline;
                     if (*pole>=*upline+sx)
                       *pole=*upline+sx-1;
                     if (i==*upline)
                       continue;
                     if (i+1==*upline)
                     {
                       if (*pole==j)
                         scrollup(x+1, y+1, sx-2, sy);
                       else
                       { scrollup(x+2, y+1, sx-3, sy);
                         linecol=col1;
                         p=line(*pole, &linecol);
                         cnline(x+1, y+1, p, sy);
                         if (strlen(p)<sy)
                           cnline(x+1, y+1+strlen(p), SPACESTR, sy-strlen(p));
                       }
                       linecol=col1;
                       p=line(*upline+sx-1, &linecol);
                       cnline(x+sx, y+1, p, sy);
                       continue;
                     }
                     else if (i-1==*upline)
                     {
                       if (*pole==j)
                         scrolldown(x+1, y+1, sx-2, sy);
                       else
                       { scrolldown(x+1, y+1, sx-3, sy);
                         linecol=col1;
                         p=line(*pole, &linecol);
                         cnline(x+sx, y+1, p, sy);
                         if (strlen(p)<sy)
                           cnline(x+sx, y+1+strlen(p), SPACESTR, sy-strlen(p));
                       }
                       linecol=col1;
                       p=line(*upline, &linecol);
                       cnline(x+1, y+1, p, sy);
                       continue;
                     }
                     putcol(col1, x+j-i+1, y+1, sy); /* col1??? */
                     break;
#endif
        case mouse_enter:
                     if ((mousex>x) && (mousex<=x+sx) &&
                         (mousey>y) && (mousey<=y+sy))
                       if (*upline+mousex-x-1<poley)
                       { if (*pole==*upline+mousex-x-1)
                           goto vse;
                         else
                         { linecol=col1;
                           line(*pole, &linecol);
                           putcol(linecol, x+*pole-*upline+1, y+1, sy);
                           *pole=*upline+mousex-x-1;
                         }
                       }
                     break;
        case mouse_right:
        case mouse_middle:
                     if ((mousex>x) && (mousex<=x+sx) &&
                         (mousey>y) && (mousey<=y+sy))
                       if (*upline+mousex-x-1<poley)
                         *pole=*upline+mousex-x-1;
        default:     goto vse;
      }
      break;
    }
  }
vse:
  // endscroll();
  crsr_restore();
  return i;
}

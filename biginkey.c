#include "gulib.h"
#include "keyc.h"

int phonebiginkey=skyf+black,fnbiginkey=skyf+red;
int selectbiginkey=purpurf+very+white;
int xbiginkey=-1;

static int pp[]={f1c,f2c,f3c,f4c,f5c,f6c,f7c,f8c,f9c,f10c};
static int spp[]={sf1c,sf2c,sf3c,sf4c,sf5c,sf6c,sf7c,sf8c,sf9c,sf10c};
static int cp[]={cf1c,cf2c,cf3c,cf4c,cf5c,cf6c,cf7c,cf8c,cf9c,cf10c};
static int ap[]={altf1c,altf2c,altf3c,altf4c,altf5c,altf6c,altf7c,altf8c,altf9c,altf10c};

/*
HKBD hKbd = 0;
KBDINFO kbInfo;
kbInfo.cb = sizeof(KBDINFO);

rc = KbdGetStatus(&kbInfo, hKbd);

if(rc)
    return rc;

kbInfo.fsMask = KEYBOARD_BINARY_MODE | KEYBOARD_SHIFT_REPORT ;
KbdSetStatus(&kbInfo, hKbd);
*/

int biginkey(char *line, char *lines, char *linec, char *linea)
{
  int i, maxx, maxy, flen;
  int res;
  
#define niz(l) cnline(xbiginkey,2,       l,   (flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen,  l+ 6,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*2,l+12,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*3,l+18,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*4,l+24,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*5,l+30,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*6,l+36,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*7,l+42,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,2+flen*8,l+48,(flen>7) ? 6 : flen-2);\
	       cnline(xbiginkey,3+flen*9,l+54,(flen>7) ? 5 : flen-2);
  graph(&maxx, &maxy, &i);
  flen=maxy/10;
  if (xbiginkey==-1) xbiginkey=maxx-1;
  for (i=0;i<10;i++) putcol(phonebiginkey,xbiginkey,
                            i*flen+2+i/9,(i==9) ? maxy-flen*9-3 : flen-2);
  cline(xbiginkey,0,     "F1");  putcol(fnbiginkey,xbiginkey,0,2);
  cline(xbiginkey,flen,  "F2");  putcol(fnbiginkey,xbiginkey,flen,2);
  cline(xbiginkey,flen*2,"F3");  putcol(fnbiginkey,xbiginkey,flen*2,2);
  cline(xbiginkey,flen*3,"F4");  putcol(fnbiginkey,xbiginkey,flen*3,2);
  cline(xbiginkey,flen*4,"F5");  putcol(fnbiginkey,xbiginkey,flen*4,2);
  cline(xbiginkey,flen*5,"F6");  putcol(fnbiginkey,xbiginkey,flen*5,2);
  cline(xbiginkey,flen*6,"F7");  putcol(fnbiginkey,xbiginkey,flen*6,2);
  cline(xbiginkey,flen*7,"F8");  putcol(fnbiginkey,xbiginkey,flen*7,2);
  cline(xbiginkey,flen*8,"F9");  putcol(fnbiginkey,xbiginkey,flen*8,2);
  cline(xbiginkey,flen*9,"F10"); putcol(fnbiginkey,xbiginkey,flen*9,3);

#if 0
  showmouse();
  for(res=-1;;DosSleep(30))
  {  i=getshifts();
     if ((i!=-1) && (i!=res))
     { hidemouse();
       res=i;
       if (i&3) { niz(lines);}
       else if (i&4) { niz(linec);}
       else if (i&8) { niz(linea);}
       else { niz(line);}
       showmouse();
     }

     if (iskey()) break;
  }
  hidemouse();
#else
  niz(line);
#endif
  res=inkey();
  if (mouse)
    if (mousex==xbiginkey)
      switch (res)
      { case  mouse_left :  res=pp [mousey/flen];break;
        case smouse_left :  res=spp[mousey/flen];break;
        case cmouse_left :  res=cp [mousey/flen];break;
        case altmouse_left: res=ap [mousey/flen];break;
      }

  switch(res)
  { case f1c: case altf1c: case sf1c: case cf1c: i=0; break;
    case f2c: case altf2c: case sf2c: case cf2c: i=1; break;
    case f3c: case altf3c: case sf3c: case cf3c: i=2; break;
    case f4c: case altf4c: case sf4c: case cf4c: i=3; break;
    case f5c: case altf5c: case sf5c: case cf5c: i=4; break;
    case f6c: case altf6c: case sf6c: case cf6c: i=5; break;
    case f7c: case altf7c: case sf7c: case cf7c: i=6; break;
    case f8c: case altf8c: case sf8c: case cf8c: i=7; break;
    case f9c: case altf9c: case sf9c: case cf9c: i=8; break;
    case f10c:case altf10c:case sf10c:case cf10c:i=9; break;
    default: goto bret;
  }
  putcol(selectbiginkey,xbiginkey,i*flen+2+i/9,(i==9) ? maxy-flen*9-3 : flen-2);
bret:
  return res;
}

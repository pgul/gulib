#include <stdio.h>
#include <string.h>
#include "gulib.h"
#include "keyc.h"

#define scmouse_left    (0xD9<<8)
#define scmouse_right   (0xDA<<8)
#define scmouse_middle  (0xDC<<8)
#define samouse_left    (0xE9<<8)
#define samouse_right   (0xEA<<8)
#define samouse_middle  (0xEC<<8)
#define camouse_left    (0xF1<<8)
#define camouse_right   (0xF2<<8)
#define camouse_middle  (0xF4<<8)
#define scamouse_left   (0xF9<<8)
#define scamouse_right  (0xFA<<8)
#define scamouse_middle (0xFC<<8)

#define MAXSTR 10

static char res[MAXSTR];
static char tabscan[0x1A]={
30,48,46,32,18,33,34,35,23,36,37,38,50,49,24,25,16/*q*/,
19,31,20,22,47,17,45,21,44};
static char tabgray[10]={82,79,80,81,75,76,77,71,72,73};
static char tabfig[10] ={11,2,3,4,5,6,7,8,9,10};
static struct
       { unsigned key;
         char *   str;
         char shft;
       } tabstr[]={
{ enterc,       "Enter",0},
{ centerc,      "Enter",2},
{ aenterc,      "Enter",4},
{ spacec,       "Space"},
{ tabc,         "Tab"},
{ stabc,        "Tab",1},
{ ctabc,        "Tab",2},
{ atabc,        "Tab",4},
{ escc,         "Esc"},
{ aescc,        "Esc",4},
{ astc,         "*",8},
{ greyslashc,   "/",8},
{ greyplusc,    "+",8},
{ greyminusc,   "-",8},
{ cgreyslashc,  "/",2+8},
{ castc,        "*",2+8},
{ cgreyminusc,  "-",2+8},
{ cgreyplusc,   "+",2+8},
{ agreyslashc,  "/",4+8},
{ aastc,        "*",4+8},
{ agreyminusc,  "-",4+8},
{ agreyplusc,   "+",4+8},
{ right5c,      "Right 5",0},
{ c5c,          "num 5",2},
{ point2c,      "num/.",0},
{ leftc,        "Left",0},
{ rightc,       "Right",0},
{ upc,          "Up",0},
{ downc,        "Down",0},
{ cleftc,       "Left",2},
{ crightc,      "Right",2},
{ cupc,         "Up",2},
{ cdownc,       "Down",2},
{ aleftc,       "Left",4},
{ arightc,      "Right",4},
{ aupc,         "Up",4},
{ adownc,       "Down",4},
{ insc,         "Ins",0},
{ del2c,        "Del",0},
{ homec,        "Home",0},
{ endc,         "End",0},
{ pgupc,        "PgUp",0},
{ pgdnc,        "PgDn",0},
{ cinsc,        "Ins",2},
{ cdel2c,       "Del",2},
{ chomec,       "Home",2},
{ cendc,        "End",2},
{ cpgupc,       "PgUp",2},
{ cpgdnc,       "PgDn",2},
{ ainsc,        "Ins",4},
{ adel2c,       "Del",4},
{ ahomec,       "Home",4},
{ aendc,        "End",4},
{ apgupc,       "PgUp",4},
{ apgdnc,       "PgDn",4},
{ delc,         "BackSpc",0},
{ cdelc,        "BackSpc",2},
{ adelc,        "BackSpc",4},
{ f11c,         "F11",0},
{ f12c,         "F12",0},
{ sf11c,        "F11",1},
{ sf12c,        "F12",1},
{ cf11c,        "F11",2},
{ cf12c,        "F12",2},
{ altf11c,      "F11",4},
{ altf12c,      "F12",4},
{ mouse_enter,    "MsLeft",0},
{ mouse_right,    "MsRight",0},
{ mouse_middle,   "MsMiddle",0},
{ smouse_left,    "MsLeft",1},
{ smouse_right,   "MsRight",1},
{ smouse_middle,  "MsMiddle",1},
{ cmouse_left,    "MsLeft",2},
{ cmouse_right,   "MsRight",2},
{ cmouse_middle,  "MsMiddle",2},
{ altmouse_left,  "MsLeft",4},
{ altmouse_right, "MsRight",4},
{ altmouse_middle,"MsMiddle",4},
{ scmouse_left,   "MsLeft",3},
{ scmouse_right,  "MsRight",3},
{ scmouse_middle, "MsMiddle",3},
{ camouse_left,   "MsLeft",6},
{ camouse_right,  "MsRight",6},
{ camouse_middle, "MsMiddle",6},
{ samouse_left,   "MsLeft",5},
{ samouse_right,  "MsRight",5},
{ samouse_middle, "MsMiddle",5},
{ scamouse_left,  "MsLeft",7},
{ scamouse_right, "MsRight",7},
{ scamouse_middle,"MsMiddle",7},
{ 0x0300,       "2",2},
{ 0x071E,       "6",2},
{ 0x0C1F,       "-",2},
{ 0x2B1C,       "\\",2},
{ 0x1A1B,       "[",2},
{ 0x1B1D,       "]",2},
{ 0x2900,       "`",4},
{ 0x8100,       "-",4},
{ 0x8300,       "=",4},
{ 0x2B00,       "\\",4},
{ 0x1A00,       "[",4},
{ 0x1B00,       "]",4},
{ 0x2700,       ";",4},
{ 0x2800,       "'",4},
{ 0x3300,       ",",4},
{ 0x3400,       ".",4},
{ 0x3500,       "/",4},
{ 0xE00A,       "Enter",2+8},
{ 0xA600,       "Enter",4+8}
};

static unsigned simple[]={
0x2960,0x297E,0x0221,0x0340,0x0423,0x0524,0x0625,0x075E,
0x0826,0x092A,0x0A28,0x0B29,0x0C2D,0x0C5F,0x0D3D,0x0D2B,
0x2B5C,0x2B7C,0x1A5B,0x1A7B,0x1B5D,0x1B7D,0x273B,0x273A,
0x2827,0x2822,0x332C,0x333C,0x342E,0x343E,0x352F,0x353F
};

char *key2str(unsigned key)
{
  char ASCII=(char)key;
  char scan=(char)(key>>8);
  int  i;

  if ((ASCII>=1) && (ASCII<=0x1A))
  { /* is it ctrl/let? */
    if (scan==tabscan[ASCII-1])
    { strcpy(res,"Ctrl/");
      *(int *)(res+5)=ASCII+('A'-1);
      return res;
    }
    goto unknown;
  }
  if ((ASCII>='A') && (ASCII<='Z'))
  { /* is it shift/let? */
    if (scan==tabscan[ASCII-'A'])
    { strcpy(res,"Shift/");
      *(int *)(res+6)=ASCII;
      return res;
    }
    goto unknown;
  }
  if ((ASCII>='a') && (ASCII<='z'))
  { if (scan==tabscan[ASCII-'a'])
    {
      *(int *)res=ASCII-('a'-'A');
      return res;
    }
    goto unknown;
  }
  /* digits */
  if ((ASCII>='0') && (ASCII<='9'))
  { if (scan==tabfig[ASCII-'0'])
    { *(int *)res=ASCII;
      return res;
    }
    if (scan==tabgray[ASCII-'0'])
    { strcpy(res,"num/");
      *(int *)(res+4)=ASCII;
      return res;
    }
    goto unknown;
  }
  if (ASCII==0)
  {
    /* functional keys */
    if ((scan>=59) && (scan<=68))
    { sprintf(res,"F%d",scan-58);
      return res;
    }
    if ((scan>=84) && (scan<=93))
    { sprintf(res,"Shift/F%d",scan-83);
      return res;
    }
    if ((scan>=94) && (scan<=103))
    { sprintf(res,"Ctrl/F%d",scan-93);
      return res;
    }
    if ((scan>=104) && (scan<=113))
    { sprintf(res,"Alt/F%d",scan-103);
      return res;
    }
    /* alt/key */
    if ((scan>=120) && (scan<=128))
    { sprintf(res,"Alt/%d",scan-119);
      return res;
    }
    for (i=0;i<0x1A;i++)
      if (scan==tabscan[i])
      { strcpy(res,"Alt/");
        *(int *)(res+4)=i+'A';
        return res;
      }
  }
unknown:
  /* others */
  for (i=0;i<sizeof(simple)/sizeof(simple[0]);i++)
    if (simple[i]==key)
    { *(int *)res=ASCII;
      return res;
    }
  for (i=0;i<sizeof(tabstr)/sizeof(tabstr[0]);i++)
    if (tabstr[i].key==key)
    { res[0]=0;
      if (tabstr[i].shft & 1)
        strcpy(res,"Shift/");
      if (tabstr[i].shft & 2)
        strcat(res,"Ctrl/");
      if (tabstr[i].shft & 4)
        strcat(res,"Alt/");
      if (tabstr[i].shft & 8)
        strcat(res,"Grey ");
      strcat(res,tabstr[i].str);
      return res;
    }
  return "";
}

/*
#include <dos.h>
void main(void)
{
  unsigned k;

  do
  {
    k=inkey();
    if (*key2str(k)==0)
      printf("****** UNKNOWN ******: %X %d:%d\n",k,k>>8,(char)k);
    else
      puts(key2str(k));
  } while (k!=escc);
}
*/

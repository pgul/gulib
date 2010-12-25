#include <string.h>
#include <ctype.h>
#include "gulib.h"
#include "keyc.h"

char edit_updown=0;
int editptr=0;
int edit(int x, int y, char *sarg, int digital, int mono)
{ int ch, ptr;
  int i, ins;
  char s[90];
  if ((digital>=128) && (digital<138)) digital=-1;
  if (digital>0) { strncpy(s, sarg, digital);
		   s[digital]='.';
		   strcpy(s+digital+1, sarg+digital);
		 }
  else strcpy(s, sarg);
  if (edit_updown)
  {
    ptr=strlen(s);
    if (ptr>editptr) ptr=editptr;
  } 
  else
    ptr=0;
  crsr_save();
  crsr_show();
  for (;;)
  { cline(x, y, s);
    place(x, y+ptr);
    ch=inkey();
    ins=getshifts() & 0x80;
    if (ch==leftc) 
    { if ((ptr==0) && mono) goto retedit;
      if (ptr) --ptr;
      if (digital)
        if (ptr==digital) --ptr;
      continue;
    }
    if (ch==rightc)
    { if (mono && (s[ptr]==0)) goto retedit;
      if (s[ptr])
      { ++ptr;
        if (digital && ((s[ptr-1]<'0') || (s[ptr-1]>'9')))
          ptr--;
      }
      if (ptr==digital) ++ptr;
      continue;
    }
    if (ch==delc)
    { if (ptr==0) continue;
      if (ins)
      { for (i=ptr--; s[i]; i++)
          if (digital)
            s[((i-1)==digital) ? i-2 : i-1] = s[(i==digital) ? i+1 : i];
          else s[i-1]=s[i];
        --i;
      }
      else i=--ptr;
      s[i]=' ';
      continue;
    }
    if (ch==del2c) 
    { if (s[ptr]==0) continue;
      for (i=ptr+1; s[i]; i++)
        if (digital)
          s[((i-1)==digital) ? i-2 : i-1] = s[(i==digital) ? i+1 : i];
        else s[i-1]=s[i];
      s[i-1]=' ';
      continue;
    }
    if (ch==homec)
    { ptr=0;
      continue;
    }
    if (ch==endc)
    { ptr=0;
      while (s[ptr]) ptr++;
      --ptr;
      while (s[ptr]==' ') if(--ptr==0) break;
      if (ptr) ptr++;
      continue;
    }
    if ((ch == escc)  ||
        (ch == tabc)  ||
        (ch == enterc)||
        ((char)ch==0))
    {
      retedit:
      if (digital>0)
      { strncpy(sarg,s,digital);
        strcpy(sarg+digital,s+digital+1);
      }
      else strcpy(sarg,s);
      editptr=ptr;
      crsr_restore();
      return(ch);
    }
    if (s[ptr]==0)
      continue;
    ch &= 0xff;
    if (ch==0) continue;
    if (digital)
    { if ((ptr==0) && (ch=='-'));
      else
        if (!isdigit(ch)) continue;
    }
    if (ins)
    { for (i=0; s[i]; i++);
      --i;
      for (; i>ptr; i--)
      { if (i==digital) continue;
        s[i]=s[digital>0 ? (((i-1)==digital) ? i-2 : i-1) : i-1];
      }
    }
    s[ptr++]=ch;
    if (ptr==digital) ++ptr;
  }
}

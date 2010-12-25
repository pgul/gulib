#include <string.h>
#include "gulib.h"

void fileext(char *s, char *ext)
{ char *p, *p1;

  p=strrchr(s, '\\');
  if (p)
  { p1=strrchr(p, '/');
    if (p1) p=p1;
  }
  if (p==NULL) p=s;
  if (strchr(p, '.')!=NULL) return;
  stripspc(p);
  p+=strlen(p);
  *p++='.';
  strcpy(p, ext);
}

#include "gulib.h"
#include "string.h"

void plakat(chtype cv, int x, int y, char *line)
{ ramka(cv, x, y, 3, strlen(line)+2);
  cline(x+1, y+1, line);
}

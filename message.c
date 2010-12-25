#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "gulib.h"
#include "keyc.h"

int message(int x, int y, char *format, ...)
{ char line[100];
  va_list arg;

  va_start(arg, format);
  vsnprintf(line, sizeof(line), format, arg);
  va_end(arg);
  line[sizeof(line)-1] = '\0';
  ramka(alarm, x, y, 3, strlen(line)+2);
  cline(x+1, y+1, line);
  crsr_save();
  crsr_hide();
  x=inkey();
  crsr_restore();
  remenu();
  return x;
}

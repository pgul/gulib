#include <sys/ioctl.h>
#include "gulib.h"

static int maxx, maxy;
int was_winch = 1; /* should be set only after arrays resize */

int graph(int *x, int *y, int *colors)
{
	struct winsize winsz;
	int ret = 0;

	if (was_winch) {
		winsz.ws_col = winsz.ws_row = 0;
		ioctl (0, TIOCGWINSZ, &winsz);
		was_winch = 0;
		if (winsz.ws_col && winsz.ws_row) {
			maxx = winsz.ws_row;
			maxy = winsz.ws_col;
		} else {
			maxx = 25;
			maxy = 80;
			ret = -1;
		}
	}
	*x = maxx;
	*y = maxy;
	if (colors) *colors = 16;
	return ret;
}

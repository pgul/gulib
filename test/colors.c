#include "gulib.h"
#include "keyc.h"

int main(void)
{
	gulib_init();
	initmouse();

	plakat(white, 2, 2, " very + black ");
	plakat(bluef+white, 6, 2, " white ");
	plakat(bluef+very+white, 10, 2, " very+white ");
	plakat(very+black, 14, 2, " very + black ");
	message(20, 2, " COLOR_PAIRS: %d ", COLOR_PAIRS);
	// inkey();
	remenu();
	remenu();
	remenu();
	remenu();
	gulib_deinit();
	return 0;
}

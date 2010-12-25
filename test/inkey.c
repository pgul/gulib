#include <stdlib.h>
#include <unistd.h>
#include "../gulib.h"
#include "../keyc.h"

int main(void)
{
	int i;

	gulib_init();
	i=0;
#if 0
	showmouse();
	for (;;usleep(100000)) {
		printf("%2u %2u 0x%04x\r\n", mousex, mousey, i);
		fflush(stdout);
		i=iskey();
		if (i==0)
			askmouse();
		else {
			printf("Wait for key 0x%04x\r\n", i);
			fflush(stdout);
			i=_inkeym();
			if (i=='a') break;
		}
	}
#else
	for (;;) {
		i=inkey();
		printf("0x%04x\r\n", i);
		if (i=='a') break;
		sleep(1);
	}
#endif

	gulib_deinit();
	return 0;
}

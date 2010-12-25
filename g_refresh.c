#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gulib.h"
#include "keyc.h"

chpair *saved_scr, *visible_scr;
int updated_scr;
static int x, y;
static char macs[256];
struct acs_type acs;

void gulib_init_sem(void);
void gulib_deinit_sem(void);
void gulib_sem(int set);

int gulib_init(void)
{
	char *p;
	int i;

	if (saved_scr && visible_scr) return 1;
	if (graph(&x, &y, NULL)) {
		x=25;
		y=80;
	}
	saved_scr = calloc(x*y, sizeof(*saved_scr));
	visible_scr = calloc(x*y, sizeof(*saved_scr));
	for (i=0; i<x*y; i++)
		saved_scr[i].ch = visible_scr[i].ch = ' ';
	initscr();
	cbreak();
	noecho();
	nonl();
	// raw();
#ifdef KEY_MOUSE
	intrflush(stdscr, FALSE);
#endif
	if (has_colors()) {
		start_color();
		// fprintf(stderr, "Has colors, color pairs %d\n", COLOR_PAIRS);
		// sleep(4);
		assume_default_colors(0, 0);
	}
	keypad(stdscr, TRUE);
//#ifdef KEY_MOUSE
	meta(stdscr, TRUE);
//#endif
//	putp(tigetstr("enacs"));
	if ((p=tigetstr("acsc")) != NULL && p != (char *)-1)
		for (; p[0] && p[1]; p+=2)
			macs[(unsigned char)p[0]] = p[1];
	p=getenv("FRAMES");
	if (p && strcmp(p, "KOI8")==0) {
		acs.tl=0xa5; // '¥';
		acs.tr=0xa8; // '¨';
		acs.bl=0xab; // '«';
		acs.br=0xae; // '®';
		acs.ver=0xa1; // '¡';
		acs.hor=0xa0; // ' ';
		acs.bar0=0x8d; // '';
		acs.bar1=0x92; // '’';
		acs.bar2=0x91; // '‘';
		acs.bar3=0x90; // '';
		acs.up='^';
		acs.down='v';
	}
#if 0
	else if (p && strcmp(p, "UTF8")==0) {
		...
	}
	else if (p && strcmp(p, "ALT")==0) { /* OS2 or Linux */
		...
	}
#endif
	else { /* VT100 */
		acs.tl=macs['l'] ? ACS_ULCORNER : '+';
		acs.tr=macs['k'] ? ACS_URCORNER : '+';
		acs.bl=macs['m'] ? ACS_LLCORNER : '+';
		acs.br=macs['j'] ? ACS_LRCORNER : '+';
		acs.ver=macs['x'] ? ACS_VLINE : '|';
		acs.hor=macs['q'] ? ACS_HLINE : '-';
		acs.bar0=macs['0'] ? ACS_BLOCK : '#';
		acs.bar1=macs['h'] ? ACS_BOARD : '#';
		acs.bar2=macs['h'] ? ACS_BOARD : '#';
		acs.bar3=macs['a'] ? ACS_CKBOARD : '#';
		acs.up=macs['-'] ? ACS_UARROW : '^';
		acs.down=macs['.'] ? ACS_DARROW : 'v';
	}
#if 1
	acs.bar0 = (0xff00 | ' ');
#endif
#ifdef WITH_PTHREAD_MUTEX
	gulib_init_sem();
#endif
	return 0;
}

int gulib_deinit(void)
{
	all_remenu();
	endwin();
#ifdef WITH_PTHREAD_MUTEX
	gulib_deinit_sem();
#endif
	return 0;
}

#define putgraph(c)	{ if (!ongraph) { putp(tigetstr("smacs")); ongraph=1; } putchar(macs[c] ? macs[c] : c); }
#define putngraph(c)	{ if (ongraph)  { putp(tigetstr("rmacs")); ongraph=0; } putchar(c); }

int g_refresh(void)
{
	int i, j;
	int cur_x, cur_y, curcol, att, ch, ongraph;
	int c;

	if (!updated_scr) return 0;
#ifdef WITH_PTHREAD_MUTEX
	gulib_sem(1);
#endif
	cur_x = cur_y = -1;
	curcol = -1;
	ongraph = 0;
	for (i=0; i<x; i++) {
		for (j=0; j<y; j++) {
			if (i == x-1 && j == y-1) break;
			if (memcmp(saved_scr+i*y+j, visible_scr+i*y+j, sizeof(chpair)) == 0) {
				continue;
			}
			if (cur_x != i || cur_y !=j) {
				putp(tparm(tigetstr("cup"), i, j));
				cur_x = i;
				cur_y = j;
			}
			att = saved_scr[i*y+j].att;
			ch = saved_scr[i*y+j].ch;
			if (ch == acs.bar0 && acs.bar0 == (0xff00 | ' ')) {
				ch = ' ';
				att = ((att & 0xf) << 4) | ((att & 0xf0) >> 4) | ((att & very) ? blim : 0) | ((att & blim) ? very : 0);
			}
			if (att != curcol) {
				if (((curcol & (very | blim)) & ~att) || curcol == -1) {
					putp(tigetstr("sgr0"));
					if (ongraph) putp(tigetstr("rmacs"));
					curcol = 0;
					ongraph = 0;
				}
				if (att & very & ~curcol) putp(tigetstr("bold"));
				if (att & blim & ~curcol) putp(tigetstr("blink"));
				putp(tparm(tigetstr("setab"), (att >> 4) & 0x7));
				putp(tparm(tigetstr("setaf"), att & 0x7));
				curcol = att;
			}
			if ((ch & 0xff) != ch) {
				for (c=0; c < 128; c++)
					if (acs_map[c] == ch) {
						putgraph(c);
						break;
					}
			} else
				putngraph(ch);
			cur_y++;
		}
	}
	memcpy(visible_scr, saved_scr, x*y*sizeof(*visible_scr));
	if (ongraph) putp(tigetstr("rmacs"));
	putp(tigetstr("sgr0"));
	putp(tparm(tigetstr("cup"), curx(), cury()));
	fflush(stdout);
#ifdef WITH_PTHREAD_MUTEX
	gulib_sem(0);
#endif
	return 0;
}


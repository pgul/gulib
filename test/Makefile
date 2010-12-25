

all:	inkey qq

inkey:	inkey.o ../gulib.a
	gcc -g -Wall -o $@ $< ../gulib.a -lncurses

qq:	qq.o
	#gcc -g -Wall -L /home/gul/work/ncurses-5.2/lib -o $@ $< -lncurses
	gcc -g -Wall -o $@ $< -lncurses

colors:	colors.o ../gulib.a
	gcc -o $@ -Wall -L /home/gul/work/ncurses-5.2/lib $< ../gulib.a -lncurses

scrlmenu:	scrlmenu.o ../gulib.a
	gcc -o $@ -Wall -g $< ../gulib.a -lncurses

.c.o:	../gulib.h Makefile
	gcc -c -g -Wall -I .. -o $*.o $*.c

CC = gcc
LINK = gcc
LIB = ar
RANLIB = ranlib
CFLAGS = -c -Wall -funsigned-char -o $@
CFLAGS += -g
CFLAGS += -DWITH_PTHREAD_MUTEX=1
#CFLAGS += -DDEBUG=1

.SUFFIXES: .o

%.o:	%.c
	$(CC) $(CFLAGS) $<

All:	gulib.a

OBJS= \
    add.o \
    addspc.o \
    barfill.o \
    barputco.o \
    biginkey.o \
    cline.o \
    cursor.o \
    edit.o \
    evakuate.o \
    fileext.o \
    gmouse.o \
    graph.o \
    gt.o \
    inkey.o \
    key2str.o \
    menu.o \
    message.o \
    mkorr.o \
    mouse.o \
    mramka.o \
    mul.o \
    mutex.o \
    myalloc.o \
    myreallo.o \
    o.o \
    plakat.o \
    quere.o \
    ramka.o \
    remenu.o \
    scrgets.o \
    scrlmenu.o \
    scroll.o \
    smenu.o \
    g_refresh.o \
    sub.o
#    uppertab.o
#    fselect.o
#    gettoday.o

$(OBJS):	gulib.h keyc.h Makefile

gulib.a:	$(OBJS)
	$(LIB) -rc $@ $? && $(RANLIB) $@

clean:
	rm -f *.o *.bak

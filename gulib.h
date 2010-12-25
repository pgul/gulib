/***************************************************
 *   Useful library                                *
 * 13.03.91  add exe_write                         *
 * 20.04.91  add scr_base
 * 20.08.91  add graphic menus
 * 04.03.92  add tsr
 ***************************************************/

#include <curses.h>
//#include <slcurses.h>

typedef struct { chtype ch, att; } chpair;

#define fileexist(name) (!access(name,0))

#define REDRAW_TEXT 1
#define REDRAW_FULL 2

extern int xbiginkey;
extern char edit_updown; extern int editptr;

extern char *statusptr;

 /* structures for windows manipulations (defined in remenu.c */
struct EVAKOITEM { int x_s, y_s, sx_s, sy_s;
                   chpair *gde_s;
       };
extern struct EVAKOITEM stack[15];
extern int sp;
extern chpair *saved_scr, *visible_scr;
 /* flag for mouse operational and coordinates (inkey.c) */
extern int mouse, mousex, mousey;
extern int mouse_mode; /* when 0 right button is escc and middle is f1c */
extern struct _ramka_type
       { chtype tl, tr, bl, br, ver, hor; } ramka_type;
extern chtype bottcol, scrollcol, ramka_shade;
extern struct acs_type
	{ chtype tl, tr, bl, br, hor, ver, bar0, bar1, bar2, bar3, up, down; } acs;

 /* turn mouse on/off, turn mouse cursor on/off (inkey.c) */
void  initmouse(void);
void  close_mouse(void);
extern void showmouse(void);
extern void hidemouse(void);
int   askmouse(void);
int   m_key(void);

int  menu  (chtype cv1, chtype cv2, int x, int y, int sx, int sy, int *p, ...);
 /* ... - is triplets
      char* prompt,
      char* what to correct,
      int   field type */
int   mkorr (chtype cv1, chtype cv2, int x, int y, int sx, int sy, int psize, ...);
void  remenu(void);
void  all_remenu(void);
void  putcol(chtype cv, int x, int y, int skoko);
void  barputcol(chtype cv, int x, int y, int sx, int sy);
void  barfill  (chtype ch, int x, int y, int sx, int sy);
void  ramka    (chtype cv, int x, int y, int sx, int sy);
void  fill     (chtype ch, int x, int y, int skoko);
void  putcell  (chpair cell, int x, int y, int skoko);
void  barcell  (chpair cell, int x, int y, int sx, int sy);
void  cchar (int x, int y, chtype ch);
void  ccell (int x, int y, chpair cell);
void  cline (int x, int y, char* line);
void  cnline(int x, int y, char* line, int skoko);
 /* if digital is not zero, only digits are allowed,
    if digital is positive, it's decimal dot position.
    Exit only by keys f1c, escc, enterc, upc, downc, and, if
    mouse mode is not 0, leftc and rightc */
int   edit  (int x, int y, char *line, int digital, int mode);

void  addspc(char *str, int skoko_vsego);
void  stripspc(char *str);
 /* malloc with check retcode */
extern void * (*myalloc)(long size);
extern void (*myfree)(void * kogo);
void *_myalloc (long size);
void *myrealloc(void *kogo, long skoko_nado);
extern int (*inkey)(void);
int   _inkey(void);
int   _inkey_norefresh(void);
int   _inkeym(int refresh); /* like _inkey() but return 0 for non-key events */
int   iskey(void);
int   back_key(int ch, ...);
void  back_key_first(int key);
void  onkey(int key, void (*reaction)(void));
int   biginkey(char *l, char *sl, char *cl, char * al);
int   getshifts(void);

void  fileext(char *filename, char *extention);

char *tbez(int skoko, char *tmpline, char *in_line);
char *add(  char *a, char *b);
char *sub(  char *a, char *b);
int  gt (  char *a, char *b);
char *mul(char *res, int ressize, int drob, char *a, char *b);
char *o(char *x);

void  evakuate(int x, int y, int sx, int sy);
int   message (int x, int y, char *format, ...);
void  plakat(chtype cv, int x, int y, char *line);

void  scrgets(int x, int y, chpair *s, int skoko);
void  place(int x, int y);

int mouse_ch(int x, int y, int sx, int sy, int key);

int   smenu(chtype cv1, chtype cv2, int x, int y, int sx, int sy, int *pole,
            int *g, char *helpline,
            char * (*getline)(int nomer), int maxnomer);
int   quere(chtype col1, chtype col2, int x, int y, int strmes, int poley,
            int *pole, ...);
int   graph(int *x, int *y, int *colors);
int   mramka(chtype col, int x, int y, int sx, int sy);
int   buildramka(char chem);
char  *key2str(unsigned key);
void  drawscroll(int x, int y, int h, int vsego, int tek);
int   wherescrl(void);
void  updatescrl(int tek);
void  endscroll(void);
int   scrlmenu(chtype col1, chtype col2, int x, int y, int sx, int sy,
               char *head, int *pole, int *upline,
               int poley, char *lines[]);
int   scrlmenuf(chtype col1, chtype col2, int x, int y, int sx, int sy,
                char *head, int *pole, int *upline,
                int poley, char * (*line)(int num, chtype *col));
int  g_refresh(void);
int  gulib_init(void);
int  gulib_deinit(void);
int  curx(void);
int  cury(void);

int  crsr_save(void);
void crsr_hide(void);
int  crsr_restore(void);
void crsr_show(void);
void crsr_size(int size); /* size : hight byte - first, low byte - last line */

int  gmouse_on(void);
int  gmouse_off(void);
int  gmouse_new(void);
int  gmouse_addold(void);
int  gmouse_offold(void);
void gmouse_cancel(void);
void gmouse_offallold(void);

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>
#include "gulib.h"
#include "keyc.h"

extern int scrlmenu_redraw;

struct lemtype {
	char filename[1024];
	unsigned long size;
	int tag;
} *lem;
int poley, lemsize;
int maxx, maxy;

char *putline(int num, chtype *col)
{
	static char s[1024];
	char size[40], name[256];

	if (strchr(lem[num].filename, '/'))
		strcpy(size, "  <Dir>  ");
	else if (lem[num].size >= 1000000)
		sprintf(size, "%3lu %03lu %03lu", lem[num].size/1000000,
		        (lem[num].size/1000)%1000, lem[num].size%1000);
	else if (lem[num].size >= 1000)
		sprintf(size, "    %3lu %03lu", lem[num].size/1000, lem[num].size%1000);
	else
		sprintf(size, "        %3lu", lem[num].size);
	snprintf(name, sizeof(name), "%-20s", lem[num].filename);
	name[20]='\0';
	sprintf(s, "%s %-20s %11s %s", lem[num].tag ? "”" : " ",
	        name, size, "");
	s[maxy-2] = '\0';
	if (lem[num].tag) *col = bluef+very+yellow;
	return s;
}

static int cmplem(const void *a, const void *b)
{
  const struct lemtype *l1, *l2;

  l1 = (const struct lemtype *)a;
  l2 = (const struct lemtype *)b;
  if (strchr(l1->filename, '/'))
  { if (strchr(l2->filename, '/') == NULL)
      return -1;
    return strcmp(l1->filename, l2->filename);
  }
  if (strchr(l2->filename, '/'))
    return 1;
  return strcmp(l1->filename, l2->filename);
}

int loaddir(void)
{ char *p;
  DIR *d;
  struct dirent *df;
  struct stat st;
  char filename[1024];
  char curdir[256];

  poley=0;
  getcwd(curdir, sizeof(curdir));
  d = opendir(curdir);
  if (d==NULL)
    return 1;
  while ((df=readdir(d))!=NULL)
  {
    if (strcmp(df->d_name, ".") == 0) continue;
    strcpy(filename, curdir);
    if (curdir[strlen(curdir)-1] != '/')
      strcat(filename, "/");
    strcat(filename, df->d_name);
    if (stat(filename, &st)) continue;
    if (!S_ISDIR(st.st_mode))
    {
      if (strlen(df->d_name)<5) continue;
      p=strrchr(df->d_name, '.');
      if (p==NULL) continue;
      p++;
    }
    if (lemsize < poley+1)
    lem=realloc(lem, sizeof(*lem)*(lemsize += 10));
    if (lem==NULL)
      exit(3);
    lem[poley].size=0;
    lem[poley].tag=0;
    strcpy(lem[poley].filename, df->d_name);
    if (S_ISDIR(st.st_mode))
      strcat(lem[poley].filename, "/");
    else
      lem[poley].size=st.st_size;
    poley++;
  }
  closedir(d);
  qsort(lem, poley, sizeof(*lem), cmplem);
  return 0;
}

int main(void)
{
	int r, i, pole, topline, col;
	setlocale(LC_ALL, "");
	gulib_init();
	initmouse();
	askmouse();
	/* setmouserep(2); */
	graph(&maxx, &maxy, &col);
	evakuate(0, 0, maxx, maxy);
	pole=topline=0;
	scrollcol=bluef+white;
	loaddir();

	for (;;) {
		r=scrlmenuf(bluef+very+sky, whitef+black, 1, 0, maxx-4, maxy-2,
		            "head", &pole, &topline, poley, putline);
		switch (r) {
			case ' ':
			case spacec:
				if (lem[pole].size == 0) continue;
				lem[pole].tag^=1;
				if (pole < poley) {
					pole++;
					if (pole-topline >= maxx-1) topline++;
				}
				continue;
			case escc:
				break;
			case enterc:
				if (lem[pole].size == 0) {
					char *newpos, *p;
					lem[pole].filename[strlen(lem[pole].filename)-1] = '\0';
					newpos = NULL;
					if (strcmp(lem[pole].filename, "..")==0) {
						newpos = malloc(1024);
						getcwd(newpos, 1024-2);
						p=strrchr(newpos, '/');
						if (p) strcpy(newpos, p+1);
						strcat(newpos, "/");
					}
					chdir(lem[pole].filename);
					loaddir();
					pole = topline = 0;
					if (newpos) {
						for (; pole < poley; pole++)
							if (strcmp(lem[pole].filename, newpos)==0)
								break;
						free(newpos);
						if (pole >= poley)
							pole = 0;
						else if (pole>maxx-5)
							topline=pole-(maxx-5);
					}
					scrlmenu_redraw=REDRAW_FULL;
				} else {
					message(10, 10, " %s ", lem[pole].filename);
				}
				continue;
			default:
				switch (r & 0xff) {
					case 'c':
						for (i=0; i<poley; i++)
							lem[i].tag = 0;
						scrlmenu_redraw |= REDRAW_TEXT;
						continue;
					case 'a':
					case '*':
						for (i=0; i<poley; i++) {
							if (lem[i].size == 0)
								continue;
							lem[i].tag ^= 1;
						}
						continue;
				}
				continue;
		}
		break;
	}
	gulib_deinit();
	return 0;
}


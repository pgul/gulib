#define INTR_TIMER 0x8
#define bcdbin(i) (((i)>>4)*10+(i)%0x10)
#define TIME_CHAS  1193180l /* Максимальная частота таймера */

extern unsigned int   cur_time_speed;

void far pascal set_time_speed(unsigned cntr);
void far pascal set_time(unsigned long tick);
unsigned long far pascal get_time(void);
void far pascal vosst_time_int(void);
void far pascal set_real_time(void);
void far pascal set_time_int(void interrupt (*newtimer)(void));
void far pascal delay_(unsigned milliseconds);
/* chem - pascal or cdecl, because chem(VOID) */
void far pascal budiln(long delay,void far (*chem)(void));


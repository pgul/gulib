#ifdef WITH_PTHREAD_MUTEX
#include <pthread.h>
#include "gulib.h"

static pthread_mutex_t sem_put;

void gulib_init_sem(void)
{
	pthread_mutex_init(&sem_put, NULL);
}

void gulib_deinit_sem(void)
{
	pthread_mutex_destroy(&sem_put);
}

void gulib_sem(int set)
{
	if (set)
		pthread_mutex_lock(&sem_put);
	else
		pthread_mutex_unlock(&sem_put);
}
#endif

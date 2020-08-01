#ifndef R2_TH_H
#define R2_TH_H

#include "r_types.h"

#define HAVE_PTHREAD 1

#if __WINDOWS__
#undef HAVE_PTHREAD
#define HAVE_PTHREAD 0
#define R_TH_TID HANDLE
#define R_TH_LOCK_T CRITICAL_SECTION
#define R_TH_COND_T CONDITION_VARIABLE
#define R_TH_SEM_T HANDLE
//HANDLE

#elif HAVE_PTHREAD
#define __GNU
#include <semaphore.h>
#include <pthread.h>
#define R_TH_TID pthread_t
#define R_TH_LOCK_T pthread_mutex_t
#define R_TH_COND_T pthread_cond_t
#define R_TH_SEM_T sem_t

#else
#error Threading library only supported for pthread and w32
#endif

#define R_TH_FUNCTION(x) int (*x)(struct r_th_t *)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct r_th_sem_t {
	R_TH_SEM_T sem;
} RThreadSemaphore;

typedef struct r_th_lock_t {
	int refs;
	R_TH_LOCK_T lock;
} RThreadLock;

typedef struct r_th_cond_t {
	R_TH_COND_T cond;
} RThreadCond;

typedef struct r_th_t {
	R_TH_TID tid;
#if HAVE_PTHREAD
	pthread_mutex_t _mutex;
	pthread_cond_t _cond;
#endif
	RThreadLock *lock;
	R_TH_FUNCTION(fun);
	void *user;    // user pointer
	int running;
	int breaked;   // thread aims to be interruped
	int delay;     // delay the startup of the thread N seconds
	int ready;     // thread is properly setup
} RThread;

typedef struct r_th_pool_t {
	int size;
	RThread **threads;
} RThreadPool;

#ifdef R_API
R_API RThread *r_th_new(R_TH_FUNCTION(fun), void *user, int delay);
R_API bool r_th_start(RThread *th, int enable);
R_API int r_th_wait(RThread *th);
R_API int r_th_wait_async(RThread *th);
R_API void r_th_break(RThread *th);
R_API void *r_th_free(RThread *th);
R_API bool r_th_kill(RThread *th, bool force);
R_API bool r_th_pause(RThread *th, bool enable);
R_API bool r_th_try_pause(RThread *th);
R_API R_TH_TID r_th_self(void);

R_API RThreadSemaphore *r_th_sem_new(unsigned int initial);
R_API void r_th_sem_free(RThreadSemaphore *sem);
R_API void r_th_sem_post(RThreadSemaphore *sem);
R_API void r_th_sem_wait(RThreadSemaphore *sem);

R_API RThreadLock *r_th_lock_new(bool recursive);
R_API int r_th_lock_wait(RThreadLock *th);
R_API int r_th_lock_check(RThreadLock *thl);
R_API int r_th_lock_enter(RThreadLock *thl);
R_API int r_th_lock_leave(RThreadLock *thl);
R_API void *r_th_lock_free(RThreadLock *thl);

R_API RThreadCond *r_th_cond_new();
R_API void r_th_cond_signal(RThreadCond *cond);
R_API void r_th_cond_signal_all(RThreadCond *cond);
R_API void r_th_cond_wait(RThreadCond *cond, RThreadLock *lock);
R_API void r_th_cond_free(RThreadCond *cond);

#endif

#ifdef __cplusplus
}
#endif

#endif

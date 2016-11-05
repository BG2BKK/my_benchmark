#ifndef _LAT_PROC_H
#define _LAT_PROC_H

#include "bench.h"
#include <sys/stat.h>
#include <sys/types.h>

typedef struct proc_state {
	bench_f prepare;
	bench_f bench;
	bench_f cooldown;
	char prog[256];
} proc_state_t;

void do_ucontext(); 
void do_fork();
void do_forkexec();
double get_fork_elapse();

#endif

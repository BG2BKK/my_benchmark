#ifndef _LAT_SYSCALL_H
#define _LAT_SYSCALL_H

#include "bench.h"

typedef struct syscall_state {
	bench_f prepare;
	bench_f bench;
	bench_f cooldown;
	int fd;
	char buf[16];
} sc_state_t;

void do_getpid();
void do_getppid();
double get_syscall_elapse(sc_state_t *); 


#endif

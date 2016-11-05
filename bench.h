#ifndef __BENCH_H__
#define __BENCH_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <ucontext.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sched.h>

typedef struct rt {
	unsigned long iter;
	unsigned long elapse;
} result_t;

typedef void (*bench_f)();

unsigned long get_total_us(struct timespec *st, struct timespec *ed);
double get_latency(unsigned long elapse, int loop);
int cmp_result(const void *p1, const void *p2);
void save_result(result_t *ret, int index, unsigned long elapse, int loop);


double get_clock_elapse();
double get_loop_elapse();

void bench_operating_system_entry();
void bench_creating_process();
void bench_bandwidth_pipe();

#endif


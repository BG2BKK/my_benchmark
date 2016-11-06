#ifndef _BW_PIPE_H
#define _BW_PIPE_H

#include "bench.h"

typedef struct p_state_t {
	int fd[2];
	char *buf;
	int childpid;
	size_t wlen;
	bench_f prepare;
	bench_f bench;
	bench_f cooldown;

} pipe_state_t;


void writer(int fd, char *buf, int buflen);
void reader(int fd, char *buf, int buflen);
void do_bw_pipe_prepare(pipe_state_t *state);
void do_bench_bandwidth_pipe(pipe_state_t *state);
void do_bw_pipe_cooldown(pipe_state_t *state);
double get_bw_pipe(pipe_state_t *state);

#endif

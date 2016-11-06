#ifndef _BW_MEM_H
#define _BW_MEM_H

#include "bench.h"

typedef struct m_state_t {
	bench_f prepare;
	bench_f bench;
	bench_f cooldown;
	char *wbuf;
	char *rbuf;
	size_t bufsize;
	int iter;
} mem_state_t;


double get_bw_mem(mem_state_t *state);
void bench_bandwidth_mem();
void do_mem_prepare(mem_state_t *state);
void do_mem_clean(mem_state_t *state);
void do_mem_rd(mem_state_t *state);

#endif

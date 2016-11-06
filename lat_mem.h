#ifndef _LAT_MEM_H
#define _LAT_MEM_H

#include "bench.h"

typedef struct lat_mem_state {
	bench_f prepare;
	bench_f bench;
	bench_f cooldown;
	size_t buflen;
	size_t stride;
} lat_mem_state_t;

#endif


/*

void do_cache_rd(mem_state_t *state) {
	register int sum = 0;
	register int *p = (int *)state->rbuf;
	register int *end = (int *)(state->rbuf + state->bufsize);
	while(p < end) {
		sum += 
#define OP(i) p[i]+
			OP(0)  OP(16) OP(32) OP(48) OP(64)
			OP(80) OP(96) OP(108) OP(124) OP(144)
			OP(160) OP(176) OP(192) OP(208) OP(224) p[240];
		p += 4096;
	}
	TEMP += sum;
}
#undef OP

void do_cache_rd_miss(mem_state_t *state) {
	register int sum = 0;
	register int *p = (int *)state->rbuf;
	register int *end = (int *)(state->rbuf + state->bufsize);
	while(p < end) {
		sum += 
#define OP(i) p[i]+
			OP(0) OP(256) OP(512) OP(768) OP(1024)
			OP(1280) OP(1536) OP(1792) OP(2048) OP(2304)
			OP(2560) OP(2816) OP(3072) OP(3328) OP(3584)
			OP(3840) OP(4096) OP(4352) OP(4608) OP(4864)
			OP(5120) OP(5376) OP(5632) OP(5888) OP(6144)
			OP(6400) OP(6656) OP(6912) OP(7168) OP(7424)
			OP(7680) p[7936];
		p += 8192;
	}
	TEMP += sum;
}
#undef OP

void test_cache() {
	printf("\tBandwidth of Cache Read\n\n");
	mem_state_t state;
	state.prepare = do_mem_prepare;
	state.bench	= do_cache_rd;
	state.cooldown = do_mem_clean;
	size_t len = 128*1024;
	state.bufsize = len;
	printf("\tblocksize: %7ldKB\tbandwidth of mem read: %lf MB/s\n", len>>10, get_bw_mem(&state));
	printf("\n");
}

void test_cache_miss() {
	printf("\tBandwidth of Cache Read\n\n");
	mem_state_t state;
	state.prepare = do_mem_prepare;
	state.bench	= do_cache_rd_miss;
	state.cooldown = do_mem_clean;
	size_t len = 32768*1024;
	state.bufsize = len;
	printf("\tblocksize: %7ldKB\tbandwidth of mem read: %lf MB/s\n", len>>10, get_bw_mem(&state));
	printf("\n");
}


 */

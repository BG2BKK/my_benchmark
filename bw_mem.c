#include "bw_mem.h"
#define TARGETLEN 1024*1024*1024

int TEMP = 0;

void do_mem_prepare(mem_state_t *state) {
	
	if(state->bufsize < TARGETLEN) {
		int block = TARGETLEN / state->bufsize;
		state->bufsize = TARGETLEN / block;
		state->iter = block; 
	} else {
		state->iter = 1;
	}

	state->rbuf = (char *)malloc(state->bufsize);
	if(!state->rbuf) {
		perror("memory malloc error\n");
		exit(1);
	}

	state->wbuf = (char *)malloc(state->bufsize);
	if(!state->wbuf) {
		perror("memory malloc error\n");
		exit(1);
	}

	bzero(state->rbuf, state->bufsize);
	bzero(state->wbuf, state->bufsize);
}

void do_mem_clean(mem_state_t *state) {
	if(state->rbuf)
		free(state->rbuf);
	if(state->wbuf)
		free(state->wbuf);
	state->iter = 0;
	state->bufsize = 0;
}

void do_mem_rd(mem_state_t *state) {
	register int sum = 0;
	register int *p = (int *)state->rbuf;
	register int *end = (int *)(state->rbuf + state->bufsize);
	while(p < end) {
		sum += 
#define OP(i) p[i]+
			OP(0) OP(4) OP(8) OP(12) OP(16)
			OP(20) OP(24) OP(28) OP(32) OP(36)
			OP(40) OP(44) OP(48) OP(52) OP(56)
			OP(60) OP(64) OP(68) OP(72) OP(76)
			OP(80) OP(84) OP(88) OP(92) OP(96)
			OP(100) OP(104) OP(108) OP(112) OP(116)
			OP(120) p[124];
		p += 128;
	}
	TEMP += sum;
}
#undef OP

void do_mem_wr(mem_state_t *state) {
	register int sum = 0;
	register int *p = (int *)state->rbuf;
	register int *end = (int *)(state->rbuf + state->bufsize);
	while(p < end) {
		sum += 
#define OP(i) p[i] = 1;
			OP(0) OP(4) OP(8) OP(12) OP(16)
			OP(20) OP(24) OP(28) OP(32) OP(36)
			OP(40) OP(44) OP(48) OP(52) OP(56)
			OP(60) OP(64) OP(68) OP(72) OP(76)
			OP(80) OP(84) OP(88) OP(92) OP(96)
			OP(100) OP(104) OP(108) OP(112) OP(116)
			OP(120) OP(124) 
		p += 128;
	}
	TEMP += sum;
}
#undef OP

void do_mem_cp(mem_state_t *state) {
	register int sum = 0;
	register int *p = (int *)state->rbuf;
	register int *d = (int *)state->wbuf;
	register int *end = (int *)(state->rbuf + state->bufsize);
	while(p < end) {
#define OP(i) d[i] = p[i];
			OP(0) OP(4) OP(8) OP(12) OP(16)
			OP(20) OP(24) OP(28) OP(32) OP(36)
			OP(40) OP(44) OP(48) OP(52) OP(56)
			OP(60) OP(64) OP(68) OP(72) OP(76)
			OP(80) OP(84) OP(88) OP(92) OP(96)
			OP(100) OP(104) OP(108) OP(112) OP(116)
			OP(120) OP(124) 
		p += 128;
		d += 128;
	}
}
#undef OP

double get_bw_mem(mem_state_t *state) {
	struct timespec st, ed;
	unsigned long elapse;
	int iter, loopcnt, loop = 40;
	int index = loop >> 2;

	bw_result_t *r = (bw_result_t *)malloc(sizeof(bw_result_t) * loop);
	loopcnt = loop;

	if(state->prepare)
		state->prepare(state);

	while(loopcnt --) {
		iter = state->iter;
		clock_gettime(CLOCK_REALTIME, &st);
		while(iter--)
			state->bench(state);
		clock_gettime(CLOCK_REALTIME, &ed);

		elapse = get_total_us(&st, &ed);
		save_bw_result(r, loopcnt, elapse, state->bufsize * state->iter);
	}

	if(state->cooldown)
		state->cooldown(state);

	qsort(r, loop, sizeof(bw_result_t ), cmp_bw_result);
	double bandwidth = get_bandwidth(r[index].elapse, r[index].volume);
	return bandwidth;
}

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

void bench_mem_rd() {
	printf("\tBandwidth of Memory Read\n\n");
	mem_state_t state;
	state.prepare = do_mem_prepare;
	state.bench	= do_mem_rd;
	state.cooldown = do_mem_clean;

	for(size_t len = 4*1024; len < 256*1024*1024; len = len << 1) {
		state.bufsize = len;
		printf("\tblocksize: %7ldKB\tbandwidth of mem read: %lf MB/s\n", len>>10, get_bw_mem(&state));
	}
	printf("\n");

}

void bench_mem_wr() {
	printf("\tBandwidth of Memory Write\n\n");
	mem_state_t state;
	state.prepare = do_mem_prepare;
	state.bench	= do_mem_wr;
	state.cooldown = do_mem_clean;

	for(size_t len = 4*1024; len < 256*1024*1024; len = len << 1) {
		state.bufsize = len;
		printf("\tblocksize: %7ldKB\tbandwidth of mem write: %lf MB/s\n", len>>10, get_bw_mem(&state));
	}
	printf("\n");
}

void bench_mem_cp() {
	printf("\tBandwidth of Memory Copy\n\n");
	mem_state_t state;
	state.prepare = do_mem_prepare;
	state.bench	= do_mem_cp;
	state.cooldown = do_mem_clean;

	for(size_t len = 4*1024; len < 256*1024*1024; len = len << 1) {
		state.bufsize = len;
		printf("\tblocksize: %7ldKB\tbandwidth of mem copy: %lf MB/s\n", len>>10, get_bw_mem(&state));
	}
	printf("\n");
}
void bench_bandwidth_mem() {

	printf("\nbenchmark on Bandwidth of Memory\n");
	printf("-------------------------------------\n\n");

//	bench_mem_rd();
//	bench_mem_wr();
//	bench_mem_cp();
	test_cache();
	printf("%d\n", TEMP);
}

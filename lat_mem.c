#include "lat_mem.h"
#include "bw_mem.h"

extern int TEMP;

static int ITER = 1000000;

static int get_iteration() {
	return ITER;
}

void do_l1_prepare(lat_mem_state_t *state) {

}

void do_l1_cooldown(lat_mem_state_t *state) {

}

void do_l1_latency(lat_mem_state_t *state) {

}

double get_cache_latency(lat_mem_state_t *state) {
	struct timespec st, ed;
	unsigned long elapse;
	int loopcnt, loop = 50;
	int index = loop >> 2;
	int iter, iteration = get_iteration();

	result_t *r = (result_t *)malloc(sizeof(result_t) * loop);
	loopcnt = loop;

	while(loopcnt --) {

		if(state->prepare)
			state->prepare(state);

		iter = iteration;
		clock_gettime(CLOCK_REALTIME, &st);
		while(iter--) {
			state->bench(state);
		};
		clock_gettime(CLOCK_REALTIME, &ed);
		elapse = get_total_us(&st, &ed);
		save_result(r, loopcnt, elapse, iteration);

		if(state->cooldown)
			state->cooldown(state);
	}

	qsort(r, loop, sizeof(result_t ), cmp_result);
	double latency = get_latency(r[index].elapse * 1000, r[index].iter); // convert us to ns
	return latency;
}

double bench_l1_latency() {
	lat_mem_state_t state;
	memset(&state, 0, sizeof(lat_mem_state_t));
	state.prepare = do_l1_prepare;
	state.bench = do_l1_latency;
	state.cooldown = do_l1_cooldown;
	printf("L1 Cache Latency: %lfns\n", get_cache_latency(&state));
}

void bench_cache_latency() {
	printf("\nbenchmark on Cache Latency\n");
	printf("-------------------------------------\n");

	bench_l1_latency();

	printf("\n--------------end--------------------\n");
}

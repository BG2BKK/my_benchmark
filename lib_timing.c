#include "bench.h"

unsigned long get_total_us(struct timespec *st, struct timespec *ed) {
	unsigned long elapse = (ed->tv_sec - st->tv_sec) * 1000000 
								+ (ed->tv_nsec - st->tv_nsec)/1000;
	return elapse;
}

double get_latency(unsigned long elapse, int loop) {
	double latency = elapse * 1. / loop ;
	return latency;
}

int cmp_result(const void *p1, const void *p2) {
	result_t *r1 = (result_t *)p1;
	result_t *r2 = (result_t *)p2;
	return (r1->elapse > r2->elapse)? 1 : ((r1->elapse < r2->elapse)? -1 : 0);
}

void save_result(result_t *ret, int index, unsigned long elapse, int loop) {
	ret[index].elapse = elapse;
	ret[index].iter = loop;
}

#define LOOP_TEN(p) p p p p p p p p p p  

static int ITER = 100000;
static int get_iteration() {
	return ITER;
}

double get_clock_elapse() {
	struct timespec st, ed, dummy;
	unsigned long elapse;
	int loopcnt, loop = 20;
	int index = loop >> 2;
	int iter, iteration = get_iteration();

	result_t *r = (result_t *)malloc(sizeof(result_t) * loop);
	loopcnt = loop;

	while(loopcnt --) {

		iter = iteration;
		clock_gettime(CLOCK_REALTIME, &st);
		while(iter--) {
			LOOP_TEN(clock_gettime(CLOCK_REALTIME, &dummy);)
		};
		clock_gettime(CLOCK_REALTIME, &ed);
		elapse = get_total_us(&st, &ed);
		save_result(r, loopcnt, elapse, iteration);
	}

	qsort(r, loop, sizeof(result_t ), cmp_result);

	double latency = get_latency(r[index].elapse, r[index].iter);
	return latency / 10;
}

double get_loop_elapse() {
	struct timespec st, ed;
	unsigned long iter, iteration;
	long *p = (long *)&p;
	double latency1, latency2, latency;
	int loop = 100;
	int loopcnt = loop;
	int index = loop >> 2;
	result_t *r_one = (result_t *)malloc(sizeof(result_t) * loop);
	result_t *r_two = (result_t *)malloc(sizeof(result_t) * loop);

//	iteration = get_iteration();
	iteration = 1000000;

	while(loopcnt --) {
		iter = iteration;
		clock_gettime(CLOCK_REALTIME, &st);
		while(iter--) {
			p = (long *)*p;
		};
		clock_gettime(CLOCK_REALTIME, &ed);
		save_result(r_one, loopcnt, 
				get_total_us(&st, &ed), iteration);

		iter = iteration;
		clock_gettime(CLOCK_REALTIME, &st);
		while(iter--) {
			p = (long *)*p;
			p = (long *)*p;
		};
		clock_gettime(CLOCK_REALTIME, &ed);
		save_result(r_two, loopcnt, 
				get_total_us(&st, &ed), iteration);
	}

	qsort(r_one, loop, sizeof(result_t ), cmp_result);
	qsort(r_two, loop, sizeof(result_t ), cmp_result);

	latency1 = get_latency(r_one[index].elapse, r_one[index].iter);
	latency2 = get_latency(r_two[index].elapse, r_two[index].iter);
	latency = 2*latency1 - latency2;
	if(latency < 0.0001)
		latency = 0;
	
	free(r_one);
	free(r_two);

	return latency;
}


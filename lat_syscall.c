
#include "lat_syscall.h"

static int ITER = 100000;
const char *f_zero = "/dev/zero";
const char *f_null = "/dev/null";

static int get_iteration() {
	return ITER;
}

void do_getpid() {
	getpid();
}

void do_getppid() {
	getppid();
}

void do_read_prepare(sc_state_t *state) {
	state->fd = open(f_zero, 0);
}

void do_read(sc_state_t *state) {
	if(read(state->fd, state->buf, 1) != 1) {
		perror("/dev/zero");
		exit(1);
	}
}
void do_read_clean(sc_state_t *state) {
	close(state->fd);
}

void do_write_prepare(sc_state_t *state) {
	state->fd = open(f_null, 1);
}

void do_write(sc_state_t *state) {
	if(write(state->fd, state->buf, 1) != 1) {
		perror("/dev/null");
		exit(1);
	}
}

void do_write_clean(sc_state_t *state) {
	close(state->fd);
}

double get_syscall_elapse(sc_state_t *state) {
	struct timespec st, ed;
	unsigned long elapse;
	int loopcnt, loop = 20;
	int index = loop >> 2;
	int iter, iteration = get_iteration();

	result_t *r = (result_t *)malloc(sizeof(result_t) * loop);
	loopcnt = loop;


	while(loopcnt --) {

		if(state->prepare) {
			state->prepare(state);
		}

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
	double latency = get_latency(r[index].elapse, r[index].iter);
	return latency;
}

void reset_state(sc_state_t *state) {
	memset(state, 0, sizeof(sc_state_t));
}

void bench_operating_system_entry() {
	printf("\nbenchmark on Operating system entry\n");
	printf("-------------------------------------\n");

	sc_state_t state;
	reset_state(&state);
	state.prepare = NULL;
	state.bench = do_getpid;
	state.cooldown = NULL;
	printf("getpid() cost: %lfus\n", get_syscall_elapse(&state));

	reset_state(&state);
	state.bench = do_getppid;
	printf("getppid() cost: %lfus\n", get_syscall_elapse(&state));

	reset_state(&state);
	state.prepare = do_write_prepare;
	state.bench = do_write;
	state.cooldown = do_write_clean;
	printf("write 1 byte to /dev/null cost: %lfus\n", get_syscall_elapse(&state));

	reset_state(&state);
	state.prepare = do_read_prepare;
	state.bench = do_read;
	state.cooldown = do_read_clean;
	printf("read 1 byte from /dev/zero cost: %lfus\n", get_syscall_elapse(&state));

	printf("\n--------------end--------------------\n");
}


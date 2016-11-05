
#include "lat_proc.h"
#define STACK_SIZE 4096
int pid;
const char *PROG = "./hello.o";
static ucontext_t uctx_main, uctx_fsm;
static char fsm_stack[STACK_SIZE];
static char main_stack[STACK_SIZE];

static int ITER = 10000;

static int get_iteration() {
	return ITER;
}

void fsm() {
	swapcontext(&uctx_fsm, &uctx_main);
}

void do_ucontext(proc_state_t *state) {
	getcontext(&uctx_fsm);
	uctx_fsm.uc_stack.ss_sp = fsm_stack;
	uctx_fsm.uc_stack.ss_size = STACK_SIZE;
	uctx_fsm.uc_link = &uctx_main;
	makecontext(&uctx_fsm, (void (*)(void))fsm, 0);

	swapcontext(&uctx_main, &uctx_fsm);
}

void do_fork() {
	switch((pid = fork())) {
		case 0:
			exit(1);
			break;
		case -1:
			break;
		default:
			waitpid(pid, NULL, 0);
			break;
	}
}

int is_exec(struct stat *buf) {
	return 0;
}

void do_forkexec_prepare(proc_state_t *state) {
	char *filename = state->prog;
	struct stat buf;
	int ret = stat(filename, &buf);
	if(ret != 0 || is_exec(&buf)) {
		perror("file not exists");
		exit(1);
	}
}

void do_forkexec(proc_state_t *state) {
	switch((pid = fork())) {
		case 0:
			close(1);
			execve(state->prog, NULL, NULL);
			exit(1);
			break;
		case -1:
			break;
		default:
			waitpid(pid, NULL, 0);
			break;
	}
}

double get_proc_elapse(proc_state_t *state) {
	struct timespec st, ed;
	unsigned long elapse;
	int loopcnt, loop = 20;
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
	double latency = get_latency(r[index].elapse, r[index].iter);
	return latency;
}

void bench_creating_process() {
	printf("\nbenchmark on Creating Process\n");
	printf("-------------------------------------\n");

	proc_state_t state;
	memset(&state, 0, sizeof(proc_state_t));
	state.prepare = NULL;
	state.bench = do_fork;
	state.cooldown = NULL;
	printf("fork+exit cost: %lfus\n", get_proc_elapse(&state));

	memset(&state, 0, sizeof(proc_state_t));
	state.prepare = do_forkexec_prepare;
	state.bench = do_forkexec;
	state.cooldown = NULL;
	snprintf(state.prog, 255, "%s", PROG);
	state.prog[strlen(state.prog)] = '\0';
	printf("fork+exec cost: %lfus\n", get_proc_elapse(&state));

	memset(&state, 0, sizeof(proc_state_t));
	state.bench = do_ucontext;
	printf("swap_ucontext cost: %lfus\n", get_proc_elapse(&state));

	printf("\n--------------end--------------------\n");
}

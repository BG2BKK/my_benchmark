#include "bench.h"
#include "lat_proc.h"
#include "lat_syscall.h"

int main() {
	double loop_latency = get_loop_elapse();
	double clock_latency = get_clock_elapse();
	printf("loop cost: %lfus\tclock func cost: %lfus\n", loop_latency, clock_latency);

	bench_operating_system_entry();
	bench_creating_process();
	bench_bandwidth_pipe();
	bench_bandwidth_mem();

	bench_cache_latency();
}


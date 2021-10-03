#include "execution_result.h"

void print_execution_result(int program_status, long long int program_execution_time, long int program_memory_usage)
{
	if (program_execution_time == -1) {
		printf("TLE\n");
		return;
	}
	if (WIFSIGNALED(program_status)) {
		printf("RE\n%d\n", WTERMSIG(program_status));
	} else if (WIFEXITED(program_status)) {
		printf("OK\n%d\n", WEXITSTATUS(program_status));
	}
	printf("%lld\n", program_execution_time);
	printf("%ld\n", program_memory_usage);
}

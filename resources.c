#include "resources.h"

int block_process_creation()
{
	struct rlimit resource_limit = {1, 1};
	return setrlimit(RLIMIT_NPROC, &resource_limit);
}

int block_file_opening()
{
	struct rlimit resource_limit = {0, 0};
	return setrlimit(RLIMIT_NOFILE, &resource_limit);
}

long int get_memory_usage()
{
	struct rusage resources_usage;
	if (getrusage(RUSAGE_CHILDREN, &resources_usage) == -1) {
		return -1;
	}
	return resources_usage.ru_maxrss;
}

int set_memory_limit(rlim_t memory_limit_in_mebibytes)
{
	const rlim_t MEBIBYTE = 1 << 20;
	struct rlimit resource_limit = {
		memory_limit_in_mebibytes * MEBIBYTE,
		memory_limit_in_mebibytes * MEBIBYTE
	};
	return setrlimit(RLIMIT_AS, &resource_limit);
}

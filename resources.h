#ifndef RESOURCES_H
#define RESOURCES_H

#include <sys/resource.h>
#include <sys/time.h>

int block_process_creation();

int block_file_opening();

long int get_memory_usage();

int set_memory_limit(rlim_t memory_limit_in_mebibytes);

#endif

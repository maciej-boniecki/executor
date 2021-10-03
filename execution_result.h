#ifndef EXECUTION_RESULT_H
#define EXECUTION_RESULT_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_execution_result(int program_status, long long int program_execution_time, long int program_memory_usage);

#endif

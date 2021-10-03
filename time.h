#ifndef TIME_H
#define TIME_H

#include <errno.h>
#include <time.h>

long long int calculate_execution_time(struct timeval start_time, struct timeval end_time);

int sleep_for(time_t miliseconds);

#endif

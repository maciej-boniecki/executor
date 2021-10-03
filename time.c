#include "time.h"

long long int calculate_execution_time(struct timeval start_time, struct timeval end_time)
{
	long long int seconds = end_time.tv_sec - start_time.tv_sec;
	long long int microseconds = end_time.tv_usec - start_time.tv_usec;
	if (microseconds < 0) {
		--seconds;
		microseconds = 1000000 - microseconds;
	}
	long long int milliseconds = seconds * 1000 + microseconds / 1000;
	if (microseconds % 1000 > 0) {
		++milliseconds;
	}
	return milliseconds;
}

int sleep_for(time_t miliseconds)
{
	struct timespec sleeping_time = {
		miliseconds / 1000,
		(miliseconds % 1000) * 1000000
	};
	int result;
	do {
		errno = 0;
		result = nanosleep(&sleeping_time, &sleeping_time);
	} while (result == -1 && errno == EINTR);
	return result;
}

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "execution_result.h"
#include "options.h"
#include "process.h"
#include "resources.h"

int main(int argc, char *argv[])
{
	struct options *options = create_options();
	if (options == NULL) {
		fprintf(stderr, "Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	if (get_options(argc, argv, options) == -1) {
		print_option_error();
		print_help(stderr, basename(argv[0]));
		exit(EXIT_FAILURE);
	}
	if (options->print_help) {
		print_help(stdout, basename(argv[0]));
		return EXIT_SUCCESS;
	}
	char *program_filename = get_program_filename(argc, argv);
	if (program_filename == NULL) {
		fprintf(stderr, "Specify program to run.\n\n");
		print_help(stderr, basename(argv[0]));
		exit(EXIT_FAILURE);
	}
	struct process *process = create_process(program_filename);
	if (process == NULL) {
		fprintf(stderr, "Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	enum errors process_configuration_error = set_process_configuration(process, options);
	if (process_configuration_error != SUCCESS) {
		print_error_message(process_configuration_error);
		exit(EXIT_FAILURE);
	}
	pid_t timer_pid = (pid_t) 0;
	if (process->execution_time_limit > 0) {
		enum errors timer_error = run_timer(process->execution_time_limit, &timer_pid);
		if (timer_error != SUCCESS) {
			print_error_message(timer_error);
			exit(EXIT_FAILURE);
		}
	}
	pid_t program_pid;
	enum errors process_error = run_process(process, &program_pid);
	if (process_error != SUCCESS) {
		if (timer_pid > (pid_t) 0) {
			kill_process(timer_pid);
		}
		print_error_message(process_error);
		exit(EXIT_FAILURE);
	}
	struct timeval start_time;
	if (gettimeofday(&start_time, NULL) == -1) {
		kill_processes(program_pid, timer_pid);
		fprintf(stderr, "Cannot get time of day.\n");
		exit(EXIT_FAILURE);
	}
	pid_t ended_process_pid;
	int ended_process_status;
	wait_for_process_end(&ended_process_pid, &ended_process_status);
	struct timeval end_time;
	if (gettimeofday(&end_time, NULL) == -1) {
		kill_processes(program_pid, timer_pid);
		fprintf(stderr, "Cannot get time of day.\n");
		exit(EXIT_FAILURE);
	}
	kill_processes(program_pid, timer_pid);
	long int memory_usage = get_memory_usage();
	if (memory_usage == -1) {
		fprintf(stderr, "Cannot get memory usage.\n");
		exit(EXIT_FAILURE);
	}
	long long int execution_time = calculate_execution_time(start_time, end_time);
	if (ended_process_pid == timer_pid) {
		if (WIFEXITED(ended_process_status) && WEXITSTATUS(ended_process_status) == EXIT_SUCCESS) {
			execution_time = -1;
		} else {
			fprintf(stderr, "Execution of timer process failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	print_execution_result(ended_process_status, execution_time, memory_usage);
	return EXIT_SUCCESS;
}

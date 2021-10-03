#ifndef PROCESS_H
#define PROCESS_H

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "errors.h"
#include "io.h"
#include "options.h"
#include "resources.h"
#include "time.h"

struct process
{
	char *program_filename;
	char *input_filename;
	char *output_filename;
	char *error_filename;
	time_t execution_time_limit;
	rlim_t memory_limit;
	uid_t user_id;
	gid_t group_id;
	int block_process_creation;
	int block_file_opening;
};

struct process *create_process(char *program_filename);

enum errors set_process_configuration(struct process *process, const struct options *options);

enum errors run_process(const struct process *process, pid_t *program_pid);

enum errors run_timer(time_t milliseconds, pid_t *timer_pid);

void wait_for_process_end(pid_t *pid, int *status);

void kill_process(pid_t pid);

void kill_processes(pid_t program_pid, pid_t timer_pid);

#endif

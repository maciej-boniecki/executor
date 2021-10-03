#include "process.h"

struct process *create_process(char *program_filename)
{
	struct process *process = (struct process *) malloc(sizeof(struct process));
	if (process != NULL) {
		process->program_filename = program_filename;
		process->input_filename = NULL;
		process->output_filename = NULL;
		process->error_filename = NULL;
		process->execution_time_limit = 0;
		process->memory_limit = 0;
		process->user_id = 0;
		process->group_id = 0;
		process->block_process_creation = 0;
		process->block_file_opening = 0;
	}
	return process;
}

enum errors set_process_configuration(struct process *process, const struct options *options)
{
	process->input_filename = options->input_filename;
	process->output_filename = options->output_filename;
	process->error_filename = options->error_filename;
	if (options->execution_time_limit != NULL) {
		errno = 0;
		process->execution_time_limit = strtol(options->execution_time_limit, NULL, 10);
		if (!(errno == 0 && process->execution_time_limit > 0)) {
			return INCORRECT_TIME_LIMIT;
		}
	}
	if (options->memory_limit != NULL) {
		errno = 0;
		process->memory_limit = strtoul(options->memory_limit, NULL, 10);
		if (!(errno == 0 && process->memory_limit > 0)) {
			return INCORRECT_MEMORY_LIMIT;
		}
	}
	if (options->user != NULL) {
		struct passwd *user = getpwnam(options->user);
		if (user == NULL) {
			return USER_NOT_FOUND;
		}
		process->user_id = user->pw_uid;
	}
	if (options->group != NULL) {
		struct group *group = getgrnam(options->group);
		if (group == NULL) {
			return GROUP_NOT_FOUND;
		}
		process->group_id = group->gr_gid;
	}
	process->block_process_creation = options->block_process_creation;
	process->block_file_opening = options->block_file_opening;
	return SUCCESS;
}

enum errors redirect_streams(const struct process *process)
{
	if (process->input_filename != NULL) {
		if (redirect_input_stream(process->input_filename, STDIN_FILENO) == -1) {
			return CANNOT_REDIRECT_INPUT_STREAM;
		}
	}
	if (process->output_filename != NULL) {
		if (redirect_output_stream(STDOUT_FILENO, process->output_filename) == -1) {
			return CANNOT_REDIRECT_OUTPUT_STREAM;
		}
	}
	if (process->error_filename != NULL) {
		if (redirect_output_stream(STDERR_FILENO, process->error_filename) == -1) {
			return CANNOT_REDIRECT_ERROR_STREAM;
		}
	}
	return SUCCESS;
}

enum errors limit_resources(const struct process *process)
{
	if (process->memory_limit > 0) {
		if (set_memory_limit(process->memory_limit) == -1) {
			return CANNOT_SET_MEMORY_LIMIT;
		}
	}
	if (process->block_process_creation) {
		if (block_process_creation() == -1) {
			return CANNOT_BLOCK_PROCESS_CREATION;
		}
	}
	if (process->block_file_opening) {
		if (block_file_opening() == -1) {
			return CANNOT_BLOCK_FILE_OPENING;
		}
	}
	return SUCCESS;
}

enum errors change_persona(const struct process *process)
{
	if (process->group_id > 0) {
		if (setgid(process->group_id) == -1) {
			return CANNOT_SET_GROUP;
		}
	}
	if (process->user_id > 0) {
		if (setuid(process->user_id) == -1) {
			return CANNOT_SET_USER;
		}
	}
	return SUCCESS;
}

enum errors configure_process(const struct process *process)
{
	enum errors error = SUCCESS;
	error = redirect_streams(process);
	if (error != SUCCESS) {
		return error;
	}
	error = limit_resources(process);
	if (error != SUCCESS) {
		return error;
	}
	error = change_persona(process);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

void run_program(char *program_filename)
{
	char *argv[] = {
		program_filename,
		NULL
	};
	execv(argv[0], argv);
}

enum errors run_process(const struct process *process, pid_t *process_pid)
{
	FILE *pipe_streams[2];
	if (create_pipe_streams(pipe_streams) == -1) {
		return CANNOT_CREATE_PIPE_STREAMS;
	}
	*process_pid = fork();
	if (*process_pid == (pid_t) -1) {
		return CANNOT_FORK_PROCESS;
	}
	enum errors error;
	if (*process_pid == (pid_t) 0) {
		fclose(pipe_streams[0]);
		error = configure_process(process);
		if (fprintf(pipe_streams[1], "%d", error) < 0) {
			exit(EXIT_FAILURE);
		}
		fclose(pipe_streams[1]);
		if (error == SUCCESS) {
			run_program(process->program_filename);
		}
		exit(EXIT_FAILURE);
	}
	fclose(pipe_streams[1]);
	if (fscanf(pipe_streams[0], "%u", &error) != 1) {
		return CANNOT_READ_PIPE_STREAM;
	}
	fclose(pipe_streams[0]);
	return error;
}

enum errors run_timer(time_t milliseconds, pid_t *timer_pid)
{
	*timer_pid = fork();
	if (*timer_pid == (pid_t) -1) {
		return CANNOT_FORK_PROCESS;
	}
	if (*timer_pid == (pid_t) 0) {
		if (sleep_for(milliseconds) == -1) {
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	return SUCCESS;
}

void wait_for_process_end(pid_t *pid, int *status)
{
	do {
		*pid = wait(status);
	} while (!WIFEXITED(*status) && !WIFSIGNALED(*status));
}

void kill_process(pid_t pid)
{
	if (kill(pid, SIGKILL) == 0) {
		waitpid(pid, NULL, 0);
	}
}

void kill_processes(pid_t program_pid, pid_t timer_pid)
{
	kill_process(program_pid);
	if (timer_pid > (pid_t) 0) {
		kill_process(timer_pid);
	}
}

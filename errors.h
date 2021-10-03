#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

enum errors {
	SUCCESS,
	CANNOT_CREATE_PIPE_STREAMS,
	CANNOT_FORK_PROCESS,
	CANNOT_REDIRECT_INPUT_STREAM,
	CANNOT_REDIRECT_OUTPUT_STREAM,
	CANNOT_REDIRECT_ERROR_STREAM,
	CANNOT_SET_MEMORY_LIMIT,
	CANNOT_BLOCK_PROCESS_CREATION,
	CANNOT_BLOCK_FILE_OPENING,
	CANNOT_SET_USER,
	CANNOT_SET_GROUP,
	CANNOT_READ_PIPE_STREAM,
	USER_NOT_FOUND,
	GROUP_NOT_FOUND,
	INCORRECT_TIME_LIMIT,
	INCORRECT_MEMORY_LIMIT
};

void print_error_message(enum errors error);

#endif
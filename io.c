#include "io.h"

int create_pipe_streams(FILE *pipe_streams[])
{
	int pipe_fds[2];
	if (pipe(pipe_fds) == -1) {
		return -1;
	}
	pipe_streams[0] = fdopen(pipe_fds[0], "r");
	if (pipe_streams[0] == NULL) {
		return -1;
	}
	pipe_streams[1] = fdopen(pipe_fds[1], "w");
	if (pipe_streams[1] == NULL) {
		return -1;
	}
	return 0;
}

int redirect_input_stream(const char *input_filename, int input_stream_fileno)
{
	int input_fileno = open(input_filename, O_RDONLY);
	if (input_fileno == -1) {
		return -1;
	}
	if (dup2(input_fileno, input_stream_fileno) == -1) {
		return -1;
	}
	close(input_fileno);
	return 0;
}

int redirect_output_stream(int output_stream_fileno, const char *output_filename)
{
	int flags = O_WRONLY | O_CREAT | O_TRUNC;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	int output_fileno = open(output_filename, flags, mode);
	if (output_fileno == -1) {
		return -1;
	}
	if (dup2(output_fileno, output_stream_fileno) == -1) {
		return -1;
	}
	close(output_fileno);
	return 0;
}

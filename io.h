#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int create_pipe_streams(FILE *pipe_streams[]);

int redirect_input_stream(const char *input_filename, int input_stream_fileno);

int redirect_output_stream(int output_stream_fileno, const char *output_filename);

#endif

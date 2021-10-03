#ifndef OPTIONS_H
#define OPTIONS_H

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options
{
	char *input_filename;
	char *output_filename;
	char *error_filename;
	char *execution_time_limit;
	char *memory_limit;
	char *user;
	char *group;
	int block_process_creation;
	int block_file_opening;
	int print_help;
};

struct options *create_options();

int get_options(int argc, char *argv[], struct options *options);

void print_option_error();

void print_help(FILE *stream, char *program_filename);

char *get_program_filename(int argc, char *argv[]);

#endif

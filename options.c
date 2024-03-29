#include "options.h"

struct options *create_options()
{
	struct options *options = (struct options *) malloc(sizeof(struct options));
	if (options != NULL) {
		options->input_filename = NULL;
		options->output_filename = NULL;
		options->error_filename = NULL;
		options->execution_time_limit = NULL;
		options->memory_limit = NULL;
		options->user = NULL;
		options->group = NULL;
		options->block_process_creation = 0;
		options->block_file_opening = 0;
		options->print_help = 0;
	}
	return options;
}

int get_options(int argc, char *argv[], struct options *options)
{
	opterr = 0;
	optind = 1;
	optopt = 0;
	int option = 0;
	while ((option = getopt(argc, argv, "i:o:e:t:m:u:g:pfh")) != -1) {
		switch (option)
		{
			case 'i':
				options->input_filename = optarg;
				break;
			case 'o':
				options->output_filename = optarg;
				break;
			case 'e':
				options->error_filename = optarg;
				break;
			case 't':
				options->execution_time_limit = optarg;
				break;
			case 'm':
				options->memory_limit = optarg;
				break;
			case 'u':
				options->user = optarg;
				break;
			case 'g':
				options->group = optarg;
				break;
			case 'p':
				options->block_process_creation = 1;
				break;
			case 'f':
				options->block_file_opening = 1;
				break;
			case 'h':
				options->print_help = 1;
				break;
			case '?':
				return -1;
		}
	}
	return 0;
}

void print_option_error()
{
	switch (optopt)
	{
		case 'i':
		case 'o':
		case 'e':
		case 't':
		case 'm':
		case 'u':
		case 'g':
			fprintf(stderr, "Option -%c requires value.\n\n", optopt);
			break;
		default:
			if (isprint(optopt)) {
				fprintf(stderr, "Unknown option -%c.\n\n", optopt);
			} else {
				fprintf(stderr, "Unknown option.\n\n");
			}
			break;
	}
}

char *get_program_filename(int argc, char *argv[])
{
	if (optind < argc) {
		return argv[optind];
	} else {
		return NULL;
	}
}

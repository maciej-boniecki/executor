#include "help.h"

void print_help(FILE *stream, char *program_filename)
{
	char *message = "Usage: %s [options] program\n"
		"\n"
		"Options:\n"
		"\t-i file\t\tRedirect file to standard input stream.\n"
		"\t-o file\t\tRedirect standard output stream to file.\n"
		"\t-e file\t\tRedirect standard error stream to file.\n"
		"\t-t n\t\tSet execution time limit to n milliseconds.\n"
		"\t-m n\t\tSet memory limit to n mebibytes.\n"
		"\t-u user\t\tSet effective user to user.\n"
		"\t-g group\tSet effective group to grup.\n"
		"\t-p\t\tBlock creation of processes.\n"
		"\t-f\t\tBlock opening of files.\n"
		"\t-h\t\tPrint this message.\n";
	fprintf(stream, message, program_filename);
}

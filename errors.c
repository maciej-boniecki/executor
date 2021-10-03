#include "errors.h"

char *error_messages[] = {
	"Success.",
	"Cannot create pipe.",
	"Cannot fork process.",
	"Cannot redirect standard input stream.",
	"Cannot redirect standard output stream.",
	"Cannot redirect standard error stream.",
	"Cannot set memory limit.",
	"Cannot block creation of processes.",
	"Cannot block opening of files.",
	"Cannot set effective user.",
	"Cannot set effective group.",
	"Cannot read from pipe.",
	"Specified user was not found.",
	"Specified group was not found.",
	"Specified execution time limit is incorrect.",
	"Specified memory limit is incorrect."
};

void print_error_message(enum errors error)
{
	fprintf(stderr, "%s\n", error_messages[error]);
}

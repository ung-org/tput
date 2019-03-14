#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXIT_USAGE_ERROR	2
#define EXIT_UNKNOWN_TERMINAL	3
#define EXIT_INVALID_OPERAND	4
#define DEFAULT_TERMINAL	"ECMA-48"

void
usage(const char *argv0, int status)
{
	printf("Usage: %s [-T type] <clear | init | reset>...\n", argv0);
	exit(status);
}

void
clear(const char *term)
{
	if (!strcmp(term, "ECMA-48")) {
		/* ESC[0;0;\x48 : Move cursor to 0,0 */
		/* ESC[\x4a : Erase to end of page */
		printf("\033[0;0;\x48\033[\x4a");
	} else {
		exit(EXIT_UNKNOWN_TERMINAL);
	}
}

void
reset(const char *term)
{
	if (!strcmp(term, "ECMA-48")) {
		/* This may or may not be right */
		printf("\033[\x63");
	} else {
		exit(EXIT_UNKNOWN_TERMINAL);
	}
}

void
init(const char *term)
{
	if (!strcmp(term, "ECMA-48")) {
		/* I don't know */
	} else {
		exit(EXIT_UNKNOWN_TERMINAL);
	}
}

int
main(int argc, char **argv)
{
	char *term = NULL, c;
	while ((c = getopt(argc, argv, "T:")) != -1) {
		switch (c) {
		case 'T':
			/* :type Specifies the terminal type.
		 	If not specified, the environment variable TERM
			will be used. If TERM is NULL or not set, an
			unspecified default will be used.
			On UNG, the default is ECMA-48. */
			term = optarg;
			break;
		default:
			usage(argv[0], EXIT_USAGE_ERROR);
		}
	}
	
	if (optind > argc) {
		usage(argv[0], EXIT_USAGE_ERROR);
	}

	if (term == NULL) {
		term = getenv("TERM");
		if (term == NULL) {
			term = DEFAULT_TERMINAL;
		}
	}

	while (argv[optind]) {
		if (!strcmp(argv[optind], "clear")) {
			clear(term);
		} else if (!strcmp(argv[optind], "init")) {
			init(term);
		} else if (!strcmp(argv[optind], "reset")) {
			reset(term);
		} else {
			usage(argv[0], EXIT_INVALID_OPERAND);
		}
		optind++;
	}
	
	return 0;
}

#include "util.h"

void err(char *m)
{
	perror(m);
	exit(EXIT_FAILURE);
}

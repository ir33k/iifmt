/* Format out file printing timestamp as readable data.

Environment variables:

IIFMT String in strftime(3) formant used to print message timestamp.
IIMARK Space separated list of words to highlight.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOLD	"\x1B[1m"	// For time
#define DIM	"\x1B[2m"	// For server messages
#define INVERT	"\x1B[7m"	// For mark
#define RESET	"\x1B[0m"

static int
starts_with(char *str, char *starts)
{
	unsigned len;

	len = strlen(starts);

	if (strlen(str) < len)
		return 0;

	return !strncmp(str, starts, len);
}

static char *
next_word(char **str)
{
	char *word = *str;

	if (!*word)
		return 0;

	while (**str > ' ')
		(*str)++;

	if (**str) {
		**str = 0;
		(*str)++;
		while (**str && **str <= ' ')
			(*str)++;
	}

	return word;
}

int main() {
	char time[32], buf[BUFSIZ], *bp, *word, *env, *fmt="%H:%M", **mark=0;
	time_t timestamp;
	struct tm *tm;
	int i, mn=0;

	if ((env = getenv("IIFMT")))
		fmt = env;

	if ((env = getenv("IIMARK"))) {
		mn = 1;
		for (i=0; env[i]; i++)
			if (env[i] == ' ')
				mn++;

		mark = malloc(mn * sizeof(*mark));

		i = 0;
		while ((word = next_word(&env)))
			mark[i++] = word;
	}

	while ((bp = fgets(buf, sizeof buf, stdin))) {
		word = next_word(&bp);
		timestamp = atoi(word);
		tm = localtime(&timestamp);
		strftime(time, sizeof time, fmt, tm);

		// Server message
		if (starts_with(bp, "-!- ")) {
			printf(DIM"%s %s"RESET, time, bp);
			continue;
		}

		printf(BOLD"%s"RESET, time);

		while ((word = next_word(&bp))) {
			printf(" ");

			for (i=0; i<mn; i++)
				if (!strcmp(mark[i], word))
					break;

			if (i<mn)
				printf(INVERT"%s"RESET, word);
			else
				printf("%s", word);
		}
		printf("\n");
	}
	return 0;
}

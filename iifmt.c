/* Format out file printing timestamp as readable data.

Environment variables:

IIFMT String in strftime(3) formant used to print message timestamp.
IIMARK Space separated list of words to highlight.
IIMAXW Number of columns before line wrap, default to 80, -1 to disable.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOLD	"\x1B[1m"
#define DIM	"\x1B[2m"
#define INVERT	"\x1B[7m"
#define RESET	"\x1B[0m"

static int starts_with(char *str, char *starts);
static char *next_word(char **str);

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

	while ((unsigned char)**str > ' ')
		(*str)++;

	if (**str) {
		**str = 0;
		(*str)++;
		while (**str && (unsigned char)**str <= ' ')
			(*str)++;
	}

	return word;
}

int main() {
	char time[32], buf[BUFSIZ], *bp, *word, *env, *fmt, **mark;
	time_t timestamp;
	struct tm *tm;
	int i, j, mn, len, w, maxw;

	fmt = "%H:%M";
	mark = 0;
	mn = 0;
	w = 0;
	maxw = 80;

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

	if ((env = getenv("IIMAXW")))
		maxw = atoi(env);

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

		printf(DIM"%s"RESET, time);

		len = strlen(time);
		w = len;

		for (i=0; (word = next_word(&bp)); i++) {
			len = strlen(word);
			w += len +1;	// +1 for space

			if (maxw > 0 && w > maxw) {
				printf("\n");
				w = len;

				// Add indentation for wrapped line
				len = strlen(time) +1;
				w += len;
				for (j=0; j<len; j++)
					printf(" ");
			} else {
				printf(" ");
			}

			for (j=0; j<mn; j++)
				if (!strcmp(mark[j], word))
					break;

			if (j<mn)	// Mark
				printf(INVERT"%s"RESET, word);
			else if (i == 0)	// Author
				printf(BOLD"%s"RESET, word);
			else
				printf("%s", word);
		}
		printf("\n");
	}
	return 0;
}

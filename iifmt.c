#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
	char time[32], msg[BUFSIZ], *fmt="%H:%M", *env;
	time_t timestamp;
	struct tm *tm;
	int i, len, max=72;

	if ((env = getenv("IIFMT_FMT")))
		fmt = env;

	if ((env = getenv("IIFMT_MAX")))
		max = atoi(env);

	while (scanf("%ld %[^\n]s\n", &timestamp, msg) != EOF) {
		tm = localtime(&timestamp);
		strftime(time, sizeof time, fmt, tm);
		printf("%s %.*s\n", time, max, msg);

		len = (int)strlen(msg);

		/* Use time buffer for indentation */
		for (i=0; time[i]; i++)
			time[i] = ' ';

		/* Wrap lines longer than MAX */
		for (i = max; len - i > 0; i += max)
			printf("%s %.*s\n", time, max, msg+i);
	}
	return 0;
}

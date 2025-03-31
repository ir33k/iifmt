/* Format out file printing timestamp as readable data. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	char time[32], msg[BUFSIZ], *fmt="%H:%M", *env;
	time_t timestamp;
	struct tm *tm;

	if ((env = getenv("IIFMT")))
		fmt = env;

	while (scanf("%ld %[^\n]s\n", &timestamp, msg) != EOF) {
		tm = localtime(&timestamp);
		strftime(time, sizeof time, fmt, tm);
		printf("%s %s\n", time, msg);
	}
	return 0;
}

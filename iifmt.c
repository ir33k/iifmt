#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
	char time[32], msg[BUFSIZ];
	time_t timestamp;
	struct tm *tm;
	int i, len, max=72;

	while (scanf("%ld %[^\n]s\n", &timestamp, msg) != EOF) {
		tm = localtime(&timestamp);
		strftime(time, sizeof time, "%H:%M", tm);
		printf("%s %.*s\n", time, max, msg);

		/* Wrap lines longer than MAX */
		len = (int)strlen(msg);
		for (i = max; len - i > 0; i += max)
			printf("      %.*s\n", max, msg+i);
	}
	return 0;
}

/* Post message to IRC 'in' channel file

Use linenoise readline like library for input.
*/
#
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include "linenoise.h"

int main() {
	char cwd[BUFSIZ], prompt[BUFSIZ], *line;
	FILE *fp;

	if (access("in", W_OK))
		err(1, "Missing 'in' file");

	getcwd(cwd, sizeof cwd);
	sprintf(prompt, "%s: ", basename(cwd));

	linenoiseHistorySetMaxLen(16);

	while ((line = linenoise(prompt))) {
		if (!*line) {
			free(line);
			continue;
		}

		linenoiseHistoryAdd(line);

		fp = fopen("in", "w");
		if (!fp)
			err(1, "fopen in");

		fprintf(fp, "%s\n", line);

		free(line);

		if (fclose(fp))
			err(1, "fclose in");
	}
	putchar('\n');
	return 0;
}

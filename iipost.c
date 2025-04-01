/* Post message to IRC 'in' channel file

Use linenoise readline like library for input.
*/

#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <err.h>
#include "linenoise.h"

#define PATH "in"

int main() {
	char cwd[BUFSIZ], prompt[BUFSIZ], *line;
	FILE *fp;

	if (access(PATH, W_OK))
		err(1, "Missing '"PATH"' file");

	getcwd(cwd, sizeof cwd);
	sprintf(prompt, "%s: ", basename(cwd));

	while (1) {
		fputs("\x1B[H" "\x1B[2J", stdout);	// Clear screen

		line = linenoise(prompt);
		if (!line)
			break;

		if (!*line)
			continue;

		linenoiseHistoryAdd(line);

		fp = fopen(PATH, "w");
		if (!fp)
			err(1, "fopen");

		fprintf(fp, "%s\n", line);

		if (fclose(fp))
			err(1, "fclose");		
	}
	putchar('\n');
	return 0;
}

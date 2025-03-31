/* Post message to in file */

#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <err.h>

#define PATH "in"

int main() {
	char buf[BUFSIZ], *parent;
	FILE *fp;

	if (access(PATH, W_OK))
		err(1, "Missing '"PATH"' file");

	getcwd(buf, sizeof buf);
	parent = basename(buf);

	while (1) {
		printf("%s: ", parent);

		if (!fgets(buf, sizeof buf, stdin))
			break;

		fp = fopen(PATH, "w");
		if (!fp)
			err(1, "fopen");

		if (fputs(buf, fp) < 0)
			err(1, "fputs");

		if (fclose(fp))
			err(1, "fclose");		
	}
	putchar('\n');
	return 0;
}

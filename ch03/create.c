#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int fd;
	char msg[] = "a new file\n";

	fd = create("/tmp/afile", OWRITE, 0664);
	if(fd < 0)
		sysfatal("create: %r");

	write(fd, msg, strlen(msg));

	close(fd);
	exits(nil);
}

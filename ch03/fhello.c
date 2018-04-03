#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	char msg[] = "hello\n";
	int fd;

	fd = open("/tmp/afile", OWRITE);
	if(fd < 0)
		sysfatal("open: %r");

	write(fd, msg, strlen(msg));

	close(fd);
	exits(nil);
}

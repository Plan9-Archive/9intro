#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int fd;

	fd = open("/tmp/afile", OWRITE | OTRUNC);
	if(fd < 0)
		sysfatal("open error: %r");

	write(fd, "hello\n", 6);

	close(fd);
	exits(nil);
}

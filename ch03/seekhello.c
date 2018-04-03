#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	int fd;

	fd = open("/tmp/afile", OWRITE);
	if(fd < 0)
		sysfatal("open error: %r");

	seek(fd, 32, 0);
	write(fd, "there\n", 6);

	close(fd);
	exits(nil);
}

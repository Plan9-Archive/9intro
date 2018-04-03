#include <u.h>
#include <libc.h>

void
main(int, char *[])
{
	char buffer[1024];
	int fd, nr;

	fd = open("/dev/cons", ORDWR);
	nr = read(fd, buffer, sizeof buffer);
	write(fd, buffer, nr);
	close(fd);

	exits(nil);
}
